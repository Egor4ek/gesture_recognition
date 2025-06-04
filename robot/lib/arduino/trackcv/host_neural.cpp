#include "Trackcv.h"
#include "host_module_iface.h"
#include <string.h>

Neural cached_neural;
Neural_script_id _script_id = Neural_script_id_dummy;

bool trackcv_neural_save_face(char* name) {
	cmp_write_map(cmp_send_ptr, 3);
		cmp_write_uint(cmp_send_ptr, 0);
			cmp_write_uint(cmp_send_ptr, MODE_NEURAL);
		cmp_write_uint(cmp_send_ptr, 1);
			cmp_write_uint(cmp_send_ptr, NEURAL_SAVE_FACE);
		cmp_write_uint(cmp_send_ptr, 2);
			cmp_write_str(cmp_send_ptr, name, strlen(name));

	return trackcv_request();
}

bool trackcv_neural_clear_faces() {
	cmp_write_map(cmp_send_ptr, 2);
		cmp_write_uint(cmp_send_ptr, 0);
			cmp_write_uint(cmp_send_ptr, MODE_NEURAL);
		cmp_write_uint(cmp_send_ptr, 1);
			cmp_write_uint(cmp_send_ptr, NEURAL_CLEAR_FACES);

	return trackcv_request();
}

bool get_cached_neural() {
	if((cached_neural.time + cached_neural.timeout) > TS_ms()) {
		// __log->debug("cache is fresh yet (timeout:%d, time:%d, current:%d)\n", cached_neural.timeout, cached_neural.time, TS_ms());
		return true;
	}
	
	__log->debug("cache is old, request new\n");
	
	cmp_write_map(cmp_send_ptr, 3);
		cmp_write_uint(cmp_send_ptr, 0);
			cmp_write_uint(cmp_send_ptr, MODE_NEURAL);
		cmp_write_uint(cmp_send_ptr, 1);
			cmp_write_uint(cmp_send_ptr, NEURAL_GET);
		cmp_write_uint(cmp_send_ptr, 2);
			cmp_write_uint(cmp_send_ptr, _script_id);
	
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
			case Neural_key_timeout:
				cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_neural.timeout);
				__log->debug("timeout: %d\n", cached_neural.timeout);
			break;
					
			case Neural_key_objects:
				cmp_read_array(cmp_recv_ptr, (uint32_t*)&cached_neural.count);
				__log->debug("face count: %d\n", cached_neural.count);
				for(uint32_t object_idx = 0; object_idx < cached_neural.count; object_idx++) {
					int32_t object_map_size;
					if ((res = cmp_read_map(cmp_recv_ptr, (uint32_t*)&object_map_size)) != 0) {
						__log->error("msgpack map read error %d\n", res);
						return false;
					}
					__log->debug("map size: %d\n", object_map_size);
					for(int32_t object_key_idx = 0; object_key_idx < object_map_size; object_key_idx++) {
						int32_t object_key;
						cmp_read_uint(cmp_recv_ptr, (uint32_t*)&object_key);
						switch(object_key) {
							case Neural_object_key_x:
								cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_neural.objects[object_idx].x);
							break;
							case Neural_object_key_y:
								cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_neural.objects[object_idx].y);
							break;
							case Neural_object_key_width:
								cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_neural.objects[object_idx].width);
							break;
							case Neural_object_key_height:
								cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_neural.objects[object_idx].height);
							break;
							case Neural_object_key_classes:
								cmp_read_array(cmp_recv_ptr, (uint32_t*)&cached_neural.objects[object_idx].class_count);
								__log->debug("class count: %d\n", cached_neural.objects[object_idx].class_count);
								for(uint32_t class_idx = 0; class_idx < cached_neural.objects[object_idx].class_count; class_idx++) {
									int32_t class_map_size;
									if ((res = cmp_read_map(cmp_recv_ptr, (uint32_t*)&class_map_size)) != 0) {
										__log->error("msgpack map read error %d\n", res);
										return false;
									}

									__log->debug("map size: %d\n", class_map_size);
									for(int32_t class_key_idx = 0; class_key_idx < class_map_size; class_key_idx++) {
										int32_t class_key;
										cmp_read_uint(cmp_recv_ptr, (uint32_t*)&class_key);
										switch(class_key) {
											case Neural_class_key_id:
												cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_neural.objects[object_idx].classes[class_idx].id);
											break;
											case Neural_class_key_p:
												cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_neural.objects[object_idx].classes[class_idx].p);
											break;
											case Neural_class_key_meta:
												uint32_t meta_size = MAX_META_SIZE;

												if(!cmp_read_str(cmp_recv_ptr, (char*)&cached_neural.objects[object_idx].classes[class_idx].meta, &meta_size)) {
													return false;
												}
											break;
										}
									}
								}
							break;
						}
					}
				}
			break;
			
			case Neural_class_key_text:
			uint32_t size = MAX_META_SIZE;
			    if(!cmp_read_str(cmp_recv_ptr, (char*)&cached_neural.text, &size)) {
				    return false;
					}
					break;
					
			

			case Neural_class_key_text_eng:
			uint32_t size2 = MAX_META_SIZE;
			    if(!cmp_read_str(cmp_recv_ptr, (char*)&cached_neural.text, &size2)) {
				    return false;
					}
										break;
		}
	}
	
	cached_neural.time = TS_ms();
	
	__log->debug("return\n");
	
	__set_errno(ERR_OK);
	
	return true;
}

void trackcv_neural_start(Neural_script_id script_id) {
	_script_id = script_id;
	
	cmp_write_map(cmp_send_ptr, 3);
	cmp_write_uint(cmp_send_ptr, 0);
		cmp_write_uint(cmp_send_ptr, MODE_NEURAL);
	cmp_write_uint(cmp_send_ptr, 1);
		cmp_write_uint(cmp_send_ptr, NEURAL_START);
	cmp_write_uint(cmp_send_ptr, 2);
		cmp_write_uint(cmp_send_ptr, _script_id);

	trackcv_request();
}

void trackcv_neural_stop() {
	cmp_write_map(cmp_send_ptr, 2);
	cmp_write_uint(cmp_send_ptr, 0);
		cmp_write_uint(cmp_send_ptr, MODE_NEURAL);
	cmp_write_uint(cmp_send_ptr, 1);
		cmp_write_uint(cmp_send_ptr, NEURAL_STOP);

	trackcv_request();
}

uint32_t trackcv_neural_count() {
	get_cached_neural();

	return cached_neural.count;
}

uint32_t trackcv_neural_x(uint32_t index) {
	get_cached_neural();
	
	if (index >= cached_neural.count) {
		__set_errno(WRONG_INDEX);
		return NEURAL_X_DEFAULT;
	}
	
	return cached_neural.objects[index].x;
}

uint32_t trackcv_neural_y(uint32_t index) {
	get_cached_neural();
	
	if (index >= cached_neural.count) {
		__set_errno(WRONG_INDEX);
		return NEURAL_Y_DEFAULT;
	}
	
	return cached_neural.objects[index].y;
}

uint32_t trackcv_neural_width(uint32_t index) {
	get_cached_neural();
	
	if (index >= cached_neural.count) {
		__set_errno(WRONG_INDEX);
		return NEURAL_WIDTH_DEFAULT;
	}
	
	return cached_neural.objects[index].width;
}

uint32_t trackcv_neural_height(uint32_t index) {
	get_cached_neural();
	
	if (index >= cached_neural.count) {
		__set_errno(WRONG_INDEX);
		return NEURAL_HEIGHT_DEFAULT;
	}
	
	return cached_neural.objects[index].height;
}

uint32_t trackcv_neural_class_count(uint32_t index) {
	get_cached_neural();
	
	if (index >= cached_neural.count) {
		__set_errno(WRONG_INDEX);
		return NEURAL_COUNT_DEFAULT;
	}

	return cached_neural.objects[index].class_count;
}

uint32_t trackcv_neural_class_id(uint32_t index, uint32_t class_index) {
	get_cached_neural();
	
	if (index >= cached_neural.count) {
		__set_errno(WRONG_INDEX);
		return NEURAL_CLASS_ID_DEFAULT;
	}

	if (class_index >= cached_neural.objects[index].class_count) {
		__set_errno(WRONG_INDEX);
		return NEURAL_CLASS_ID_DEFAULT;
	}

	return cached_neural.objects[index].classes[class_index].id;
}

uint32_t trackcv_neural_class_p(uint32_t index, uint32_t class_index) {
	get_cached_neural();
	
	if (index >= cached_neural.count) {
		__set_errno(WRONG_INDEX);
		return NEURAL_CLASS_P_DEFAULT;
	}

	if (class_index >= cached_neural.objects[index].class_count) {
		__set_errno(WRONG_INDEX);
		return NEURAL_CLASS_P_DEFAULT;
	}

	return cached_neural.objects[index].classes[class_index].p;
}

char* trackcv_neural_class_meta(uint32_t index, uint32_t class_index) {
	get_cached_neural();
	
	if (index >= cached_neural.count) {
		__set_errno(WRONG_INDEX);
		return 0;
	}

	if (class_index >= cached_neural.objects[index].class_count) {
		__set_errno(WRONG_INDEX);
		return 0;
	}

	return cached_neural.objects[index].classes[class_index].meta;
}

char* trackcv_neural_class_text() {
get_cached_neural();
  return cached_neural.text;
}
