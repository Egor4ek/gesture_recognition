#include "log.hpp"

extern "C" {
	#include "xprintf.h"
}

void Logger::log(LOG_LEVEL level, const char* fmt, va_list arp) {
	if(level > this->level) {
		return;
	}
	
	COLOR color = COLOR_DEFAULT;
	switch(level) {
		case LOG_ERROR:
			color = COLOR_RED;
			break;
		case LOG_WARNING:
			color = COLOR_YELLOW;
			break;
		case LOG_INFO:
			color = COLOR_GREEN;
			break;
		default:
			color = COLOR_DEFAULT;
	}
	
	if(colorize) {
		xfprintf(putfunc, "\x1b[%dm", (int)color);
	}

	xfvprintf(putfunc, fmt, arp);
	
	if(colorize) {
		xfprintf(putfunc, "\x1b[0m");
	}
}