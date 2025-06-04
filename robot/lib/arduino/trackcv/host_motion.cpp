#include "Trackcv.h"
#include "host_module_iface.h"

Motions cached_motions;

bool get_cached_motions() {
	if((cached_motions.time + cached_motions.timeout) > TS_ms()) {
		// __log->debug("cache is fresh yet (timeout:%d, time:%d, current:%d)\n", cached_motions.timeout, cached_motions.time, TS_ms());
		return true;
	}
	
	__log->debug("cache is old, request new\n");
	
	cmp_write_map(cmp_send_ptr, 2);
		cmp_write_uint(cmp_send_ptr, 0);
			cmp_write_uint(cmp_send_ptr, MODE_MOTION);
		cmp_write_uint(cmp_send_ptr, 1);
			cmp_write_uint(cmp_send_ptr, GET);
	
	/* send request, parse response */
	if(!trackcv_request()) {
		return false;
	}
	
	int32_t root_size = 0;
	int32_t res;

	if ((res = cmp_read_map(cmp_recv_ptr, (uint32_t*)&root_size)) != 0) {
		__log->error("msgpack map read error %d\n", res);
		return false;
	}
	
	if (root_size == 0) {
		__set_errno(MODULE_NOREADY);
		__log->warn("module not ready\n");
		return false;
	}
	
	for(int32_t key_idx = 0; key_idx < root_size; key_idx++) {
		int32_t key;
		cmp_read_uint(cmp_recv_ptr, (uint32_t*)&key);
		switch(key) {
			case Motions_key_timeout:
				cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_motions.timeout);
				__log->debug("timeout: %d\n", cached_motions.timeout);
			break;
			case Motions_key_motions:
				cmp_read_array(cmp_recv_ptr, (uint32_t*)&cached_motions.count);
				__log->debug("motion count: %d\n", cached_motions.count);
				for(uint32_t motion_idx = 0; motion_idx < cached_motions.count; motion_idx++) {
					int32_t motion_map_size;
					if ((res = cmp_read_map(cmp_recv_ptr, (uint32_t*)&motion_map_size)) != 0) {
						__log->error("msgpack map read error %d\n", res);
						return false;
					}
					__log->debug("map size: %d\n", motion_map_size);
					for(int32_t motion_key_idx = 0; motion_key_idx < motion_map_size; motion_key_idx++) {
						int32_t motion_key;
						cmp_read_uint(cmp_recv_ptr, (uint32_t*)&motion_key);
						switch(motion_key) {
							case Motion_key_x:
								cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_motions.motions[motion_idx].x);
							break;
							case Motion_key_y:
								cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_motions.motions[motion_idx].y);
							break;
							case Motion_key_width:
								cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_motions.motions[motion_idx].width);
							break;
							case Motion_key_height:
								cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_motions.motions[motion_idx].height);
							break;
						}
					}
				}
			break;
		}
	}
	
	cached_motions.time = TS_ms();
	
	__log->debug("return\n");
	
	__set_errno(ERR_OK);
	
	return true;
}

void trackcv_motion_start() {
	// emit_module("FACE START");
}

void trackcv_motion_stop() {
	// emit_module("FACE STOP");
}

uint32_t trackcv_motion_count() {
	get_cached_motions();
	
	return cached_motions.count;
}

uint32_t trackcv_motion_x(uint32_t index) {
	get_cached_motions();
	
	if (index >= cached_motions.count) {
		__set_errno(WRONG_INDEX);
		return MOTION_X_DEFAULT;
	}
	
	return cached_motions.motions[index].x;
}

uint32_t trackcv_motion_y(uint32_t index) {
	get_cached_motions();
	
	if (index >= cached_motions.count) {
		__set_errno(WRONG_INDEX);
		return MOTION_Y_DEFAULT;
	}
	
	return cached_motions.motions[index].y;
}

uint32_t trackcv_motion_width(uint32_t index) {
	get_cached_motions();
	
	if (index >= cached_motions.count) {
		__set_errno(WRONG_INDEX);
		return MOTION_WIDTH_DEFAULT;
	}
	
	return cached_motions.motions[index].width;
}

uint32_t trackcv_motion_height(uint32_t index) {
	get_cached_motions();
	
	if (index >= cached_motions.count) {
		__set_errno(WRONG_INDEX);
		return MOTION_HEIGHT_DEFAULT;
	}
	
	return cached_motions.motions[index].height;
}
