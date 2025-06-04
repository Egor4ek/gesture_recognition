#include "Trackcv.h"
#include "host_module_iface.h"

Lines cached_lines;

bool get_cached_lines() {
	if((cached_lines.time + cached_lines.timeout) > TS_ms()) {
		return true;
	}
	
	__log->debug("cache is old, request new\n");
	
	cmp_write_map(cmp_send_ptr, 2);
		cmp_write_uint(cmp_send_ptr, 0);
			cmp_write_uint(cmp_send_ptr, MODE_LINE);
		cmp_write_uint(cmp_send_ptr, 1);
			cmp_write_uint(cmp_send_ptr, LINE_GET);
	
	/* send request, parse response */
	if(!trackcv_request()) {
		return false;
	}
	
	volatile int32_t root_size = 0;
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
	
	for(uint32_t key_idx = 0; key_idx < root_size; key_idx++) {
		int32_t key;
		cmp_read_uint(cmp_recv_ptr, (uint32_t*)&key);
		switch(key) {
			case Lines_key_timeout:
				cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_lines.timeout);
				__log->debug("timeout: %d\n", cached_lines.timeout);
			break;
			case Lines_key_lines:
				cmp_read_array(cmp_recv_ptr, (uint32_t*)&cached_lines.count);
				__log->debug("line count: %d\n", cached_lines.count);
				for(uint32_t line_idx = 0; line_idx < cached_lines.count; line_idx++) {
					int32_t line_map_size;
					if ((res = cmp_read_map(cmp_recv_ptr, (uint32_t*)&line_map_size)) != 0) {
						__log->error("msgpack map read error %d\n", res);
						return false;
					}
					__log->debug("map size: %d\n", line_map_size);
					for(int32_t line_key_idx = 0; line_key_idx < line_map_size; line_key_idx++) {
						int32_t line_key;
						cmp_read_uint(cmp_recv_ptr, (uint32_t*)&line_key);
						switch(line_key) {
							case Line_key_x:
								cmp_read_int(cmp_recv_ptr, (int32_t*)&cached_lines.lines[line_idx].x);
                                // __log->debug("line #%d x: %d\n", line_idx, cached_lines.lines[line_idx].x);
							break;
							case Line_key_width:
								cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_lines.lines[line_idx].width);
							break;
						}
					}
				}
			break;
		}
	}
	
	cached_lines.time = TS_ms();
	
	__set_errno(ERR_OK);
	
	return true;
}

void trackcv_line_start() {
	// emit_module("FACE START");
}

void trackcv_line_stop() {
	// emit_module("FACE STOP");
}

uint32_t trackcv_line_count() {
	get_cached_lines();
	
	return cached_lines.count;
}

int32_t trackcv_line_x(uint32_t index) {
	get_cached_lines();
	
	if (index >= cached_lines.count) {
		__set_errno(WRONG_INDEX);
		return LINE_X_DEFAULT;
	}
	
	return cached_lines.lines[index].x;
}

uint32_t trackcv_line_width(uint32_t index) {
	get_cached_lines();
	
	if (index >= cached_lines.count) {
		__set_errno(WRONG_INDEX);
		return LINE_WIDTH_DEFAULT;
	}
	
	return cached_lines.lines[index].width;
}

void trackcv_set_line_height(uint32_t line_height) {
	
}

void trackcv_set_line_min_width (uint32_t line_min_width) {
	
}

void trackcv_set_frame_filter (uint32_t frame_filter) {
	
}

void trackcv_set_line_min_fill (uint32_t line_min_fill) {
	
}