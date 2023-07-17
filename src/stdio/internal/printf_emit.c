#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "printf_impl.h"

void __putchar_emit(void *cookie, char c)
{
	(void) cookie;
	putchar(c);
}

void __emit_string(printf_emit emit, const char *str)
{
	size_t i;
	size_t len;

	len = strlen(str);

	for (i = 0; i < len; ++i)
		emit.emit(emit.cookie, str[i]);
}

void __emit_char(printf_emit emit, char c)
{
	emit.emit(emit.cookie, c);
}
