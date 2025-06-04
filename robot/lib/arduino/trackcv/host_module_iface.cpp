#include "host_module_iface.h"
#include "Trackcv.h"

#include "packet.hpp"
#include "cmp.h"
#include "cmp_shell.h"

void delay_ms(uint32_t delay);

int32_t (*__reader)(void);
bool (*__writer)(char);
Logger* __log;

Packet<200> packet_bus_recv;
char buffer_bus_send[50];
char packet_bus_send[50];

CMP_Shell cmp_recv;
CMP_Shell cmp_send;

Errno __errno = ERR_OK;

void __set_errno(Errno errno) {
	__errno = errno;
}

Errno trackcv_get_errno() {
	return __errno;
}

cmp_ctx_t* cmp_send_ptr;
cmp_ctx_t* cmp_recv_ptr;

extern Circles cached_circles;
extern Faces cached_faces;
extern QR cached_QR;
extern Color cached_color;
extern Lines cached_lines;

void reset_cache() {
	cached_circles.time = 0;
	cached_faces.time = 0;
	cached_QR.time = 0;
	cached_color.time = 0;
	cached_lines.time = 0;
}

bool trackcv_check() {
	int32_t test = 1390;
    
    __log->debug("[trackcv] check\n");
    
	cmp_write_map(cmp_send.cmp_ptr(), 2);
	cmp_write_int(cmp_send.cmp_ptr(), 0);
	cmp_write_int(cmp_send.cmp_ptr(), MODE_PING);
	cmp_write_int(cmp_send.cmp_ptr(), 1);
	cmp_write_int(cmp_send.cmp_ptr(), test);
    
    __log->debug("[trackcv] send check packet\n");
	
	if(!trackcv_request()) {
		return false;
	}
	
	int32_t root_size = 0;
	int32_t res;
	if ((res = cmp_read_map(cmp_recv.cmp_ptr(), (uint32_t*)&root_size)) != 0) {
		__log->error("msgpack map read error %d\n", res);
		return false;
	}
	
	if(root_size < 1) {
		__log->error("msgpack map size error (%d)\n", root_size);
		return false;
	}
	
	int32_t key = 0;
	int32_t test_data = 0;
	cmp_read_uint(cmp_recv_ptr, (uint32_t*)&key);
	if(key != 0) {
		__log->error("msgpack map root key error (%d)\n", key);
		return false;
	}
	cmp_read_uint(cmp_recv.cmp_ptr(), (uint32_t*)&test_data);
	if(test_data != test + 10) {
		__log->error("msgpack map answer error (%d)\n", test_data);
		return false;
	}
	
	return trackcv_get_errno() == ERR_OK;
}

bool Trackcv_init(int32_t (*reader)(void), bool (*writer)(char), int32_t (*putfunc)(unsigned char)) {	
	__reader = reader;
	__writer = writer;
    
    static Logger trackcv_log(Logger::LOG_INFO, putfunc);
    
	__log = &trackcv_log;
    
    __log->debug("[trackcv] start\n");
	
	__set_errno(ERR_OK);
	
	packet_bus_recv.init(__log);
    
    __log->debug("[trackcv] packet init\n");
	
	static Logger cmp_recv_log(*__log);
	cmp_recv_log.setLevel(Logger::LOG_WARNING);
	cmp_recv.init(
		"r",
		(uint8_t*)packet_bus_recv.getPacket(),
		packet_bus_recv.getLength(),
		cmp_recv_log
	);
    
    __log->debug("[trackcv] cmp_recv init\n");
	
	static Logger cmp_send_log(*__log);
	cmp_send_log.setLevel(Logger::LOG_WARNING);
	cmp_send.init(
		"w",
		(uint8_t*)packet_bus_send,
		sizeof(packet_bus_send)/sizeof(packet_bus_send[0]),
		cmp_send_log
	);
    
    __log->debug("[trackcv] cmp_send init\n");
	
	cmp_send_ptr = cmp_send.cmp_ptr();
	cmp_recv_ptr = cmp_recv.cmp_ptr();

	
	return trackcv_check();
	
}

bool get_response() {
	const int32_t SLEEP_INTERVAL = 2;
	
	int32_t timeout = GET_REQUEST_TIMEOUT / SLEEP_INTERVAL;
	
	int32_t ch;
	while(timeout--) {
		if((ch = __reader()) != -1) {
            __log->debug("get char: %d,", ch);
			if (packet_bus_recv.flow((unsigned char)ch)) {
				if(packet_bus_recv.checkCRC()) {
					packet_bus_recv.getPacket()[packet_bus_recv.getLength()] = '\0';
					__log->debug("packet receive: ");
					for (int32_t i = 0; i < packet_bus_recv.getLength(); i++) {
						if(packet_bus_recv.getPacket()[i] < ' ') {
							__log->debug("\\x%x", (uint8_t)packet_bus_recv.getPacket()[i]);
						} else {
							__log->debug("%c", packet_bus_recv.getPacket()[i]);
						}
					}
					__log->debug("\n");
					
					cmp_recv.Reset();
					cmp_recv.SetSize(packet_bus_recv.getLength());
					
					return true;
				} else {
					return false; // crc error
				}
			}
		} else {
			delay_ms(SLEEP_INTERVAL);
		}
	}
	
	return false; // timeout
}

void send_request() {
	int32_t outLen = makePacket(
		(char*)cmp_send.getBuffer(),
		cmp_send.getSize(),
		buffer_bus_send, sizeof(buffer_bus_send)/sizeof(buffer_bus_send[0]),
		*__log
	);
	
    // __log->debug("packet len: %d\n", outLen);
    
	for(int32_t i = 0; i < outLen; i++) {
        // __log->debug("send char %c\n", buffer_bus_send[i]);
		if(!__writer(buffer_bus_send[i])) {
			__set_errno(COMM_ERR);
			break;
		};
	}
}

bool trackcv_request() {
	int32_t retry_count = RETRY_COUNT;
	bool success = false;
    
    __log->debug("[trackcv] make request\n");
    
	while(retry_count-- && !success) {
		send_request();
		success = get_response();
		if(!success) {
			__log->warn("no response, retry %d remain \n", retry_count);
		}
	}
	if(!success) {
        __log->error("[trackcv] NO MODULE\n");
		__set_errno(NO_MODULE);
	} else {
        // __log->debug("[trackcv] module answer\n");
		__set_errno(ERR_OK);
	}
	
	cmp_send.Reset();
	
	return success;
}

void trackcv_set_frame_x(uint32_t frame_x) {
	cmp_write_map(cmp_send.cmp_ptr(), 2);
		cmp_write_int(cmp_send.cmp_ptr(), 0);
			cmp_write_int(cmp_send.cmp_ptr(), MODE_FRAME);
		cmp_write_int(cmp_send.cmp_ptr(), 1);
			cmp_write_map(cmp_send.cmp_ptr(), 1);
				cmp_write_int(cmp_send.cmp_ptr(), Frame_key_x);
					cmp_write_int(cmp_send.cmp_ptr(), frame_x);
	reset_cache();
	trackcv_request();
}

void trackcv_set_frame_y(uint32_t frame_y) {
	cmp_write_map(cmp_send.cmp_ptr(), 2);
		cmp_write_int(cmp_send.cmp_ptr(), 0);
			cmp_write_int(cmp_send.cmp_ptr(), MODE_FRAME);
		cmp_write_int(cmp_send.cmp_ptr(), 1);
			cmp_write_map(cmp_send.cmp_ptr(), 1);
				cmp_write_int(cmp_send.cmp_ptr(), Frame_key_y);
					cmp_write_int(cmp_send.cmp_ptr(), frame_y);
	
	trackcv_request();
}

void trackcv_set_frame_height(uint32_t frame_height) {
	cmp_write_map(cmp_send.cmp_ptr(), 2);
		cmp_write_int(cmp_send.cmp_ptr(), 0);
			cmp_write_int(cmp_send.cmp_ptr(), MODE_FRAME);
		cmp_write_int(cmp_send.cmp_ptr(), 1);
			cmp_write_map(cmp_send.cmp_ptr(), 1);
				cmp_write_int(cmp_send.cmp_ptr(), Frame_key_height);
					cmp_write_int(cmp_send.cmp_ptr(), frame_height);
					
	trackcv_request();
}

void trackcv_set_frame_width(uint32_t frame_width) {
	cmp_write_map(cmp_send.cmp_ptr(), 2);
		cmp_write_int(cmp_send.cmp_ptr(), 0);
			cmp_write_int(cmp_send.cmp_ptr(), MODE_FRAME);
		cmp_write_int(cmp_send.cmp_ptr(), 1);
			cmp_write_map(cmp_send.cmp_ptr(), 1);
				cmp_write_int(cmp_send.cmp_ptr(), Frame_key_width);
					cmp_write_int(cmp_send.cmp_ptr(), frame_width);
					
	trackcv_request();
	
}

// -------------- parse packet ----------------
/*
Logger cmp_log(Logger::LOG_WARNING, HAL::xputchar);

if (argc >= 3 && strcmp(argv[2], "broken_payload") == 0) test_broken_payload(cmp.cmp_ptr(), cmp_out.cmp_ptr(), log);
if (argc >= 3 && strcmp(argv[2], "run") == 0) process_message(cmp.cmp_ptr(), cmp_out.cmp_ptr(), log);

for (int32_t i = cmp_out.getSize(); i >= 0; i--) {
	cmp_out.getBuffer()[i+1] = cmp_out.getBuffer()[i];
}
cmp_out.SetSize(cmp_out.getSize() + 1);
cmp_out.getBuffer()[0] = address;

int32_t outLen = makePacket((char*)cmp_out.getBuffer(), cmp_out.getSize(), out, sizeof(out)/sizeof(out[0]), log);
connection.send(out, outLen);
*/
