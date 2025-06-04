#ifndef LOG_H
#define LOG_H

#include <stdarg.h>
#include <stdint.h>

/*!
* \file
* Модуль простого логгера
*/

class Logger {	
public:
	typedef enum {
		LOG_TRACE = 0,	// normal console operation
		LOG_INFO,		// cool stuff
		LOG_ERROR,		// errors
		LOG_WARNING,	// abnormal behavior without critical effect
		LOG_DEBUG		// developers stuff
	} LOG_LEVEL;
	
	Logger(LOG_LEVEL level, int32_t (*putfunc)(unsigned char)) {
		this->level = level;
		this->putfunc = putfunc;
		
		colorize = false;
	}
	
	Logger(const Logger& proto) {
		this->level = proto.level;
		this->putfunc = proto.putfunc;
		
		colorize = true;
	}
	
	void log(LOG_LEVEL level, const char* fmt, va_list arp);
	
	void setLevel(LOG_LEVEL level) {
		this->level = level;
	}
	
	void error(const char* fmt, ...) {
		va_list arp;
		va_start(arp, fmt);
		log(LOG_ERROR, fmt, arp);
		va_end(arp);
	}
	void info(const char* fmt, ...) {
		va_list arp;
		va_start(arp, fmt);
		log(LOG_INFO, fmt, arp);
		va_end(arp);
	}
	void warn(const char* fmt, ...) {
		va_list arp;
		va_start(arp, fmt);
		log(LOG_WARNING, fmt, arp);
		va_end(arp);
	}
	void debug(const char* fmt, ...) {
		va_list arp;
		va_start(arp, fmt);
		log(LOG_DEBUG, fmt, arp);
		va_end(arp);
	}
	void trace(const char* fmt, ...) {
		va_list arp;
		va_start(arp, fmt);
		log(LOG_TRACE, fmt, arp);
		va_end(arp);
	}
private:
	LOG_LEVEL level;
	int32_t (*putfunc)(unsigned char);
	bool colorize;
	
	typedef enum {
		COLOR_DEFAULT = 0,
		COLOR_RED = 31,
		COLOR_GREEN = 32,
		COLOR_YELLOW = 33
	} COLOR;
};

#endif