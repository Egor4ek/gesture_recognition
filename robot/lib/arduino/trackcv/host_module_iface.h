#ifndef HOST_MODULE_IFACE_H
#define HOST_MODULE_IFACE_H

#include "Trackcv.h"
#include "cmp.h"
#include "log.hpp"

const int32_t FACE_MAX_COUNT = 5;
const int32_t LINE_MAX_COUNT = 5;
const int32_t MOTION_MAX_COUNT = 5;
const int32_t NEURAL_CLASSES_MAX_COUNT = 5;
const int32_t NEURAL_MAX_COUNT = 5;

const int32_t FACE_X_DEFAULT = 0;
const int32_t FACE_Y_DEFAULT = 0;
const int32_t FACE_WIDTH_DEFAULT = 0;
const int32_t FACE_HEIGHT_DEFAULT = 0;

const int32_t LINE_X_DEFAULT = 0;
const int32_t LINE_WIDTH_DEFAULT = 0;

const int32_t NEURAL_X_DEFAULT = 0;
const int32_t NEURAL_Y_DEFAULT = 0;
const int32_t NEURAL_WIDTH_DEFAULT = 0;
const int32_t NEURAL_HEIGHT_DEFAULT = 0;
const int32_t NEURAL_COUNT_DEFAULT = 0;
const int32_t NEURAL_CLASS_ID_DEFAULT = 0;
const int32_t NEURAL_CLASS_P_DEFAULT = 0;

const int32_t GET_REQUEST_TIMEOUT = 500;
const int32_t RETRY_COUNT = 3;

const int32_t MODE_PING = 0;
const int32_t MODE_FACE = 1;
const int32_t MODE_LINE = 2;
const int32_t MODE_SQUARE = 3;
const int32_t MODE_CIRCLE = 4;
const int32_t MODE_QR = 5;
const int32_t MODE_MOTION = 6;
const int32_t MODE_COLOR = 7;
const int32_t MODE_FRAME = 8;
const int32_t MODE_NEURAL = 9;

const int32_t Frame_key_x = 0;
const int32_t Frame_key_y = 1;
const int32_t Frame_key_width = 2;
const int32_t Frame_key_height = 3;

const int32_t GET = 0;
const int32_t START = 1;
const int32_t STOP = 2;

void __set_errno(Errno errno);

bool trackcv_request();
uint32_t TS_ms(void);

extern cmp_ctx_t* cmp_send_ptr;
extern cmp_ctx_t* cmp_recv_ptr;
extern Logger* __log;

typedef struct {
	uint32_t x;
	uint32_t y;
	uint32_t R;
} Circle;
typedef enum {
	Circle_key_x = 0,
	Circle_key_y = 1,
	Circle_key_R = 2,
} Circle_key;

typedef struct {
	uint32_t count;
	uint64_t time;
	uint32_t timeout;
	Circle circles[LINE_MAX_COUNT];
} Circles;
typedef enum {
	Circles_key_timeout = 0,
	Circles_key_circles = 1,
} Circles_key;

const int32_t CIRCLE_GET = 0;
const int32_t CIRCLE_START = 1;
const int32_t CIRCLE_STOP = 2;

typedef struct {
	uint32_t count;
	uint32_t x;
	uint32_t y;
	uint32_t width;
	uint32_t height;
} Face;
typedef enum {
	Face_key_x = 0,
	Face_key_y = 1,
	Face_key_width = 2,
	Face_key_height = 3
} Face_key;

typedef struct {
	uint32_t count;
	uint64_t time;
	uint32_t timeout;
	Face faces[FACE_MAX_COUNT];
} Faces;
typedef enum {
	Faces_key_timeout = 2,
	Faces_key_faces = 3,
} Faces_key;

const int32_t FACE_GET = 0;
const int32_t FACE_START = 1;
const int32_t FACE_STOP = 2;

const int32_t MAX_QR_DATA_SIZE = 200;

const int32_t MAX_META_SIZE = 71;

typedef struct {
	uint32_t time;
	uint32_t timeout;
	
	bool found;
	uint32_t found_uniq;
	char data[MAX_QR_DATA_SIZE];
	uint32_t interval;
} QR;

typedef enum {
	QR_key_timeout = 0,
	QR_key_found = 1,
	QR_key_data = 2,
	QR_key_interval = 3
} QR_key;


const int32_t QR_GET = 0;
const int32_t QR_START = 1;
const int32_t QR_STOP = 2;


typedef struct {
	uint32_t time;
	uint32_t timeout;
	
	uint32_t hue;
	uint32_t saturation;
	uint32_t value;
	Color_code code;
} Color;
typedef enum {
	Color_key_timeout = 0,
	Color_key_hue = 1,
	Color_key_saturation = 2,
	Color_key_value = 3,
	Color_key_code = 4
} Color_key;


const int32_t COLOR_GET = 0;
const int32_t COLOR_START = 1;
const int32_t COLOR_STOP = 2;


typedef struct {
	int32_t x;
	uint32_t width;
} Line;
typedef enum {
	Line_key_x = 0,
	Line_key_width = 1,
} Line_key;

typedef struct {
	uint32_t count;
	uint64_t time;
	uint32_t timeout;
	Line lines[LINE_MAX_COUNT];
} Lines;
typedef enum {
	Lines_key_timeout = 0,
	Lines_key_lines = 1,
} Lines_key;

const int32_t LINE_GET = 0;
const int32_t LINE_START = 1;
const int32_t LINE_STOP = 2;

const int32_t LINE_SET_HEIGHT = 10;
const int32_t LINE_SET_MIN_WIDTH = 11;
const int32_t LINE_SET_FRAME_FILTER = 12;
const int32_t LINE_SET_MIN_FILL = 13;

const int32_t MOTION_X_DEFAULT = 0;
const int32_t MOTION_Y_DEFAULT = 0;
const int32_t MOTION_WIDTH_DEFAULT = 0;
const int32_t MOTION_HEIGHT_DEFAULT = 0;

typedef struct {
    uint32_t count;
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
} Motion;
typedef enum {
    Motion_key_x = 0,
    Motion_key_y = 1,
    Motion_key_width = 2,
    Motion_key_height = 3
} Motion_key;

typedef struct {
    uint32_t count;
    uint64_t time;
    uint32_t timeout;
    Motion motions[MOTION_MAX_COUNT];
} Motions;

typedef enum {
    Motions_key_timeout = 2,
    Motions_key_motions = 3,
} Motions_key;

typedef struct {
	uint32_t id;
	uint32_t p;
	char meta[MAX_META_SIZE];
} Neural_class;
typedef enum {
	Neural_class_key_id = 0,
	Neural_class_key_p = 1,
	Neural_class_key_meta = 2
} Neural_class_key;

typedef struct {
	uint32_t x;
	uint32_t y;
	uint32_t width;
	uint32_t height;
	uint32_t class_count;
	Neural_class classes[NEURAL_CLASSES_MAX_COUNT];
} Neural_object;
typedef enum {
	Neural_object_key_x = 0,
	Neural_object_key_y = 1,
	Neural_object_key_width = 2,
	Neural_object_key_height = 3,
	Neural_object_key_class_count = 4,
	Neural_object_key_classes = 5
} Neural_object_key;

typedef struct {
	uint32_t count;
	uint64_t time;
	uint32_t timeout;
	char text[MAX_META_SIZE];
	Neural_object objects[NEURAL_MAX_COUNT];
} Neural;
typedef enum {
	Neural_key_timeout = 2,
	Neural_key_objects = 3,
	Neural_class_key_text = 4,
	Neural_class_key_text_eng = 5
} Neural_key;

const int32_t NEURAL_GET = 0;
const int32_t NEURAL_START = 1;
const int32_t NEURAL_STOP = 2;
const int32_t NEURAL_SAVE_FACE = 3;
const int32_t NEURAL_CLEAR_FACES = 4;

#endif
