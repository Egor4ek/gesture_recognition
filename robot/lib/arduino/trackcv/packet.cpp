#include "packet.hpp"
#include <string.h>

Logger* _crc_log;

uint16_t gen_crc16(const uint8_t *data, uint16_t size) {
	uint16_t out = 0;
	int bits_read = 0, bit_flag;

	/* Sanity check: */
	if(data == 0)
		return 0;

	while(size > 0)
	{
		bit_flag = out >> 15;
		
		/* Get next bit: */
		out <<= 1;
		out |= (*data >> bits_read) & 1; // item a) work from the least significant bits
		
		/* Increment bit counter: */
		bits_read++;
		if(bits_read > 7)
		{
			bits_read = 0;
			data++;
			size--;
		}
		
		/* Cycle check: */
		if(bit_flag)
			out ^= CRC16;
	}
	
	// item b) "push out" the last 16 bits
	for (unsigned int i = 0; i < 16; i++) {
		bit_flag = out >> 15;
		out <<= 1;
		if(bit_flag)
			out ^= CRC16;
        
        // _crc_log->debug("calc out: %d\n", i);
	}
	
	// item c) reverse the bits
	uint16_t crc = 0;
    unsigned int i = 0x8000;
    int j = 0x0001;
	for ( ; i != 0; i >>=1, j <<= 1) {
		if (i & out) crc |= j;
        // _crc_log->debug("calc crc: %d\n", i);
	}
	
	return crc;
}

int makePacket(char* payload, int payloadLen, char* packet, int packetMaxLen, Logger& log) {
	if(packetMaxLen < 4) {
		return -1;
	}
	
	int packetLen = 0;
	packet[packetLen++] = PACKET_START;

	//strncpy(&packet[1], payload, payloadLen);
	
	for(int i = 0; i < payloadLen; i++) {
		if(packetLen > packetMaxLen) {
			return -1;
		}
		
		switch((unsigned char)payload[i]) {
			case PACKET_START:
			case PACKET_STOP:
			case SHIELD:
				packet[packetLen++] = SHIELD;
				packet[packetLen++] = payload[i] & 0x0F;
			break;
			default:
				packet[packetLen++] = payload[i];
		}
	}
	log.debug("send:");
	for (int i = 0; i < payloadLen; i++) {
		if(payload[i] < ' ') {
			log.debug("\\x%x", payload[i]);
		} else {
			log.debug("%c", payload[i]);
		}
	}
	log.debug("\n");
    
    _crc_log = &log;
	uint16_t crc = gen_crc16((uint8_t*)payload, payloadLen);
	uint8_t crc_half = crc & 0xFF;
	switch(crc_half) {
		case PACKET_START:
		case PACKET_STOP:
		case SHIELD:
			packet[packetLen++] = SHIELD;
			packet[packetLen++] = crc_half & 0x0F;
		break;
		default:
			packet[packetLen++] = crc_half;
	}
	crc_half = (crc >> 8) & 0xFF;
	switch(crc_half) {
		case PACKET_START:
		case PACKET_STOP:
		case SHIELD:
			packet[packetLen++] = SHIELD;
			packet[packetLen++] = crc_half & 0x0F;
		break;
		default:
			packet[packetLen++] = crc_half;
	}
	
	packet[packetLen++] = PACKET_STOP;
	return packetLen;
	
}