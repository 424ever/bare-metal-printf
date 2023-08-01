#include <stddef.h>
#include <stdarg.h>

#ifndef __STDIO_H_
#	define __STDIO_H_

#	define EOF -1

int printf(const char *format, ...);
int putchar(int c);
int puts(const char *s);
int snprintf(char *str, size_t size, const char *format, ...);
int sprintf(char *str, const char *format, ...);
int vsnprintf(char *str, size_t size, const char *format, va_list ap);
int vsprintf(char *str, const char *format, va_list ap);

#endif
