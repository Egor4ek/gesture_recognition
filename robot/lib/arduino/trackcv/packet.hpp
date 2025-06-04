#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include "log.hpp"

const int CRC16 = 0x8005;

const unsigned char PACKET_START = 0xF5;
const unsigned char PACKET_STOP = 0xF3;
const unsigned char SHIELD = 0xFA;

uint16_t gen_crc16(const uint8_t *data, uint16_t size);

template <int SIZE> class Packet {
public:
	
	Packet() {
		
	}
	
	void init(Logger* log) {
		this->log = log;
		stage = 0;
		shield = false;
		packetCnt = 0;
	}
	
	bool flow(uint8_t ch) {
		// log->debug("[%c:%x]\n", ch, ch);
		
		if(ch == PACKET_START) {
			// log->debug("find preamble\n");
			//dlprintf("start packet\r\n");
			stage = PACKET_START;
			packetCnt = 0;
			return false;
		}
		
		if(stage == 0) {
			log->warn("skip [%c:%d]\n", ch, ch);
			return false;
		}
		
		if(ch == PACKET_STOP) {
			//dlprintf("CRC = %d\r\n", gen_crc16(packet, packetCnt));
			stage = 0;
			// log->debug("find end, packet: %s", packet);
			return true;
		}
		
		if(ch == SHIELD) {
			//dlprintf("shield sym\r\n");
			shield = 1;
			return false;
		}

		if(shield) {
			packet[packetCnt++] = ch | 0xF0;
			shield = 0;
		} else {
			packet[packetCnt++] = ch;
		}
		
		return false;
	}
	bool checkCRC() {
		return gen_crc16((uint8_t*)packet, packetCnt) == 0;
	}
	char* getPacket() {
		return packet;
	}
	int getLength() {
		return packetCnt - 2;
	}
	
private:
	Logger* log;
	char packet[SIZE];
	int stage;
	bool shield;
	int packetCnt;
	
};

int makePacket(char* payload, int payloadLen, char* packet, int packetLen, Logger& log);

#endif