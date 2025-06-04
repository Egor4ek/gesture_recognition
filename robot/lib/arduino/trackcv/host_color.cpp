#include "Trackcv.h"
#include "host_module_iface.h"

Color cached_color;

bool get_cached_color() {
	if((cached_color.time + cached_color.timeout) > TS_ms()) {
		return true;
	}
	
	__log->debug("cache is old, request new\n");
	
	cmp_write_map(cmp_send_ptr, 2);
	cmp_write_uint(cmp_send_ptr, 0);
	cmp_write_uint(cmp_send_ptr, MODE_COLOR);
	cmp_write_uint(cmp_send_ptr, 1);
	cmp_write_uint(cmp_send_ptr, COLOR_GET);
	
	/* send request, parse response */
	if(!trackcv_request()) {
		__set_errno(NO_MODULE);
		return false;
	}
	
	int32_t root_size = 0;
	int32_t res;

	if ((res = cmp_read_map(cmp_recv_ptr, (uint32_t*)&root_size)) != 0) {
		__log->error("msgpack map read error %d\n", res);
		return false;
	}
	__log->debug("root size: %d\n", root_size);
	
	if (root_size == 0) {
		__set_errno(MODULE_NOREADY);
		__log->warn("module not ready\n");
		return false;
	}
	
	for(int32_t key_idx = 0; key_idx < root_size; key_idx++) {
		int32_t key;
		cmp_read_uint(cmp_recv_ptr, (uint32_t*)&key);
		switch(key) {
			case Color_key_timeout:
			cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_color.timeout);
			__log->debug("timeout: %d\n", cached_color.timeout);
			break;
			case Color_key_hue:
			cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_color.hue);
			break;
			case Color_key_saturation:
			cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_color.saturation);
			break;
			case Color_key_value:
			cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_color.value);
			break;
			case Color_key_code:
			cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_color.code);
			break;
		}
	}
	
	cached_color.time = TS_ms();
	
	__log->debug("return\n");
	
	__set_errno(ERR_OK);
	
	return true;
}

void trackcv_color_start() {
	// emit_module("FACE START");
}

void trackcv_color_stop() {
	// emit_module("FACE STOP");
}

uint32_t trackcv_color_hue() {
	get_cached_color();
	
	return cached_color.hue;
}

uint32_t trackcv_color_saturation() {
	get_cached_color();
	
	return cached_color.saturation;
}

uint32_t trackcv_color_value() {
	get_cached_color();
	
	return cached_color.value;
}

uint32_t  trackcv_color_code() {
	get_cached_color();
	return cached_color.code;
}
