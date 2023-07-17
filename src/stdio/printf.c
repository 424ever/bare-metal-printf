#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "internal/printf_impl.h"
#include "stdio_internal.h"

int printf(const char *format, ...)
{
	int	    len;
	va_list	    ap;
	printf_emit emit;

	__stdio_term_init();

	va_start(ap, format);

	emit.emit   = &__putchar_emit;
	emit.cookie = NULL;

	len = __printf_impl(format, ap, emit);

	va_end(ap);

	return len;
}
