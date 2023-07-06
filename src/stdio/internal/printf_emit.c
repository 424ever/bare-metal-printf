#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "printf_impl.h"

void emit_putchar(void *cookie, char c)
{
	(void)cookie;
	putchar(c);
}

void emit_string(emit_func emit, void *cookie, const char *str)
{
	size_t i;
	size_t len;

	len = strlen(str);

	for (i = 0; i < len; ++i)
		emit(cookie, str[i]);
}

