#include "Trackcv.h"
#include "host_module_iface.h"

Faces cached_faces;

bool get_cached_faces() {
	if((cached_faces.time + cached_faces.timeout) > TS_ms()) {
		// __log->debug("cache is fresh yet (timeout:%d, time:%d, current:%d)\n", cached_faces.timeout, cached_faces.time, TS_ms());
		return true;
	}
	
	__log->debug("cache is old, request new\n");
	
	cmp_write_map(cmp_send_ptr, 2);
		cmp_write_uint(cmp_send_ptr, 0);
			cmp_write_uint(cmp_send_ptr, MODE_FACE);
		cmp_write_uint(cmp_send_ptr, 1);
			cmp_write_uint(cmp_send_ptr, FACE_GET);
	
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
			case Faces_key_timeout:
				cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_faces.timeout);
				__log->debug("timeout: %d\n", cached_faces.timeout);
			break;
			case Faces_key_faces:
				cmp_read_array(cmp_recv_ptr, (uint32_t*)&cached_faces.count);
				__log->debug("face count: %d\n", cached_faces.count);
				for(uint32_t face_idx = 0; face_idx < cached_faces.count; face_idx++) {
					int32_t face_map_size;
					if ((res = cmp_read_map(cmp_recv_ptr, (uint32_t*)&face_map_size)) != 0) {
						__log->error("msgpack map read error %d\n", res);
						return false;
					}
					__log->debug("map size: %d\n", face_map_size);
					for(int32_t face_key_idx = 0; face_key_idx < face_map_size; face_key_idx++) {
						int32_t face_key;
						cmp_read_uint(cmp_recv_ptr, (uint32_t*)&face_key);
						switch(face_key) {
							case Face_key_x:
								cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_faces.faces[face_idx].x);
							break;
							case Face_key_y:
								cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_faces.faces[face_idx].y);
							break;
							case Face_key_width:
								cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_faces.faces[face_idx].width);
							break;
							case Face_key_height:
								cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_faces.faces[face_idx].height);
							break;
						}
					}
				}
			break;
		}
	}
	
	cached_faces.time = TS_ms();
	
	__log->debug("return\n");
	
	__set_errno(ERR_OK);
	
	return true;
}

void trackcv_face_start() {
	// emit_module("FACE START");
}

void trackcv_face_stop() {
	// emit_module("FACE STOP");
}

uint32_t trackcv_face_count() {
	get_cached_faces();
	
	return cached_faces.count;
}

uint32_t trackcv_face_x(uint32_t index) {
	get_cached_faces();
	
	if (index >= cached_faces.count) {
		__set_errno(WRONG_INDEX);
		return FACE_X_DEFAULT;
	}
	
	return cached_faces.faces[index].x;
}

uint32_t trackcv_face_y(uint32_t index) {
	get_cached_faces();
	
	if (index >= cached_faces.count) {
		__set_errno(WRONG_INDEX);
		return FACE_Y_DEFAULT;
	}
	
	return cached_faces.faces[index].y;
}

uint32_t trackcv_face_width(uint32_t index) {
	get_cached_faces();
	
	if (index >= cached_faces.count) {
		__set_errno(WRONG_INDEX);
		return FACE_WIDTH_DEFAULT;
	}
	
	return cached_faces.faces[index].width;
}

uint32_t trackcv_face_height(uint32_t index) {
	get_cached_faces();
	
	if (index >= cached_faces.count) {
		__set_errno(WRONG_INDEX);
		return FACE_HEIGHT_DEFAULT;
	}
	
	return cached_faces.faces[index].height;
}
