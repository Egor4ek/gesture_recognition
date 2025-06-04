/*------------------------------------------------------------------------*/
/* Universal string handler for user console interface  (C)ChaN, 2011     */
/*------------------------------------------------------------------------*/

#ifndef _STRFUNC
#define _STRFUNC

#define _USE_XFUNC_OUT	1	/* 1: Use output functions */
#define	_CR_CRLF		1	/* 1: Convert \n ==> \r\n in the output char */

#define _USE_XFUNC_IN	0	/* 1: Use input function */
#define	_LINE_ECHO		0	/* 1: Echo back input chars in xgets function */

#if _USE_XFUNC_OUT
#define xdev_out(func) xfunc_out = (void(*)(unsigned char))(func)

#include <stdarg.h>
#include <stdint.h>

void xputc (char c);
void xputs (const char* str);
void xfputs (int32_t (*func)(unsigned char), const char* str);
void xfputsn (int32_t(*func)(unsigned char),	const char*	str, int32_t size);
void xprintf (const char* fmt, ...);
void xsprintf (char* buff, const char* fmt, ...);
void xfprintf (int32_t (*func)(unsigned char), const char*	fmt, ...);
void xfvprintf (int32_t(*func)(unsigned char), const char*	fmt, va_list arp);

void put_dump (const void* buff, unsigned long addr, int32_t len, int32_t width);
#define DW_CHAR		sizeof(char)
#define DW_SHORT	sizeof(short)
#define DW_LONG		sizeof(long)
#endif

#if _USE_XFUNC_IN
#define xdev_in(func) xfunc_in = (unsigned char(*)(void))(func)
extern unsigned char (*xfunc_in)(void);
int32_t xgets (char* buff, int32_t len);
int32_t xfgets (unsigned char (*func)(void), char* buff, int32_t len);

#endif

int32_t xatoi (char** str, int32_t* res);
#ifdef PC
#define xatoi atoi
#endif

#endif
