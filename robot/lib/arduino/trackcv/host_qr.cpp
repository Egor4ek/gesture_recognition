#include "Trackcv.h"
#include "host_module_iface.h"

QR cached_QR;

bool get_cached_QR() {
    if ((cached_QR.time + cached_QR.timeout) > TS_ms()) {
        return true;
    }

    __log->debug("old cache, request new\n");

    cmp_write_map(cmp_send_ptr, 2);
    cmp_write_uint(cmp_send_ptr, 0);
    cmp_write_uint(cmp_send_ptr, MODE_QR);
    cmp_write_uint(cmp_send_ptr, 1);
    cmp_write_uint(cmp_send_ptr, QR_GET);

    /* send request, parse response */

    if (!trackcv_request()) {
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

    __log->trace("get response\n");

    for (int32_t key_idx = 0; key_idx < root_size; key_idx++) {
        int32_t key;
        cmp_read_uint(cmp_recv_ptr, (uint32_t*)&key);

        switch (key) {
            case QR_key_timeout:
                cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_QR.timeout);
                __log->debug("timeout: %d\n", cached_QR.timeout);
                break;

            case QR_key_found: {
                uint32_t found_uniq = 0;
                cmp_read_uint(cmp_recv_ptr, (uint32_t*)&found_uniq);
                __log->debug("found uniq:%d\n", found_uniq);
                if (cached_QR.found_uniq != found_uniq) {
                    cached_QR.found_uniq = found_uniq;
                    cached_QR.found = true;
                    __log->debug("found\n");
                } else {
                    __log->debug("not found\n");
                }
            } break;

            case QR_key_interval:
                cmp_read_uint(cmp_recv_ptr, (uint32_t*)&cached_QR.interval);
                break;

            case QR_key_data:

                int32_t data_size = 0;
                cmp_read_array(cmp_recv_ptr, (uint32_t*)&data_size);
                if (data_size >= MAX_QR_DATA_SIZE) {
                    return false;
                }

                for (int32_t i = 0; i < data_size; i++) {
                    cmp_read_uchar(cmp_recv_ptr, (uint8_t*)&cached_QR.data[i]);
                    // __log->debug("[%c]", cached_QR.data[i]);
                }

                cached_QR.data[data_size - 1] = 0;
                __log->debug("\ncached_QR.data:%s\n", cached_QR.data);

                break;
        }
    }

    cached_QR.time = TS_ms();

    __log->debug("return\n");

    __set_errno(ERR_OK);

    return true;
}

void trackcv_qr_start() {
    // emit_module("FACE START");
}

void trackcv_qr_stop() {
    // emit_module("FACE STOP");
}

bool trackcv_qr_found() {
    get_cached_QR();
    bool res = cached_QR.found;
    cached_QR.found = false;
    return res;
}

uint32_t trackcv_qr_interval() {
    get_cached_QR();

    return cached_QR.interval;
}

char* trackcv_qr_data() {
    get_cached_QR();

    return (char*)cached_QR.data;
}
