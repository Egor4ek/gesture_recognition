#include "Trackcv.h"
#include "host_module_iface.h"

Circles cached_circles;

bool get_cached_circles() {
	if((cached_circles.time + cached_circles.timeout) > TS_ms()) {
		return true;
	}
	
	__log->debug("cache is old, request new\n");
	
	cmp_write_map(cmp_send_ptr, 2);
		cmp_write_uint(cmp_send_ptr, 0);
			cmp_write_uint(cmp_send_ptr, MODE_CIRCLE);
		cmp_write_uint(cmp_send_ptr, 1);
			cmp_write_uint(cmp_send_ptr, CIRCLE_GET);
	
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
			case Circles_key_timeout:
				cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_circles.timeout);
				__log->debug("timeout: %d\n", cached_circles.timeout);
			break;
			case Circles_key_circles:
				cmp_read_array(cmp_recv_ptr, (uint32_t*)&cached_circles.count);
				__log->debug("circles count: %d\n", cached_circles.count);
				for(int32_t circle_idx = 0; circle_idx < cached_circles.count; circle_idx++) {
					int32_t circle_map_size;
					if ((res = cmp_read_map(cmp_recv_ptr, (uint32_t*)&circle_map_size)) != 0) {
						__log->error("msgpack map read error %d\n", res);
						return false;
					}
					__log->debug("map size: %d\n", circle_map_size);
					for(int32_t circle_key_idx = 0; circle_key_idx < circle_map_size; circle_key_idx++) {
						int32_t circle_key;
						cmp_read_uint(cmp_recv_ptr, (uint32_t*)&circle_key);
						switch(circle_key) {
							case Circle_key_x:
								cmp_read_int(cmp_recv_ptr, (int32_t*)&cached_circles.circles[circle_idx].x);
							break;
							case Circle_key_y:
								cmp_read_int(cmp_recv_ptr, (int32_t*)&cached_circles.circles[circle_idx].y);
							break;
							case Circle_key_R:
								cmp_read_int(cmp_recv_ptr, (int32_t*)&cached_circles.circles[circle_idx].R);
							break;
						}
					}
				}
			break;
		}
	}
	
	cached_circles.time = TS_ms();
	
	__log->debug("return\n");
	
	__set_errno(ERR_OK);
	
	return true;
}

void trackcv_circle_start() {
	// emit_module("FACE START");
}

void trackcv_circle_stop() {
	// emit_module("FACE STOP");
}

uint32_t trackcv_circle_count() {
	get_cached_circles();
	
	return cached_circles.count;
}

uint32_t trackcv_circle_x(uint32_t index) {
	get_cached_circles();
	
	if (index >= cached_circles.count) {
		__set_errno(WRONG_INDEX);
		return LINE_X_DEFAULT;
	}
	
	return cached_circles.circles[index].x;
}


uint32_t trackcv_circle_y(uint32_t index) {
	get_cached_circles();
	
	if (index >= cached_circles.count) {
		__set_errno(WRONG_INDEX);
		return LINE_X_DEFAULT;
	}
	
	return cached_circles.circles[index].y;
}


uint32_t trackcv_circle_R(uint32_t index) {
	get_cached_circles();
	
	if (index >= cached_circles.count) {
		__set_errno(WRONG_INDEX);
		return LINE_X_DEFAULT;
	}
	
	return cached_circles.circles[index].R;
}