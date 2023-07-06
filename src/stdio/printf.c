#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "internal/printf_impl.h"
#include "stdio_internal.h"

int printf(const char *format, ...)
{
	int	len;
	va_list ap;

	_stdio_term_init();

	va_start(ap, format);

	len = printf_impl(format, ap, &emit_putchar, NULL);

	va_end(ap);

	return len;
}

