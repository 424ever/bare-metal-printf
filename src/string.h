#include <stddef.h>

#ifndef _STRING_H_
#	define _STRING_H_

char  *strchr(const char *s, int c);
char  *strpbrk(const char *s, const char *accept);
char  *strrchr(const char *s, int c);
size_t strlen(const char *s);
void  *memcpy(void *dest, const void *src, size_t n);
void  *memmove(void *dest, const void *src, size_t n);
void  *memset(void *s, int c, size_t n);

#endif
