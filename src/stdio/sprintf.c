#include <stdio.h>

#include "internal/printf_impl.h"

int sprintf(char *str, const char *format, ...)
{
	sprintf_cookie cookie;
	va_list	       ap;
	printf_emit    emit;
	int	       len;

	va_start(ap, format);

	cookie.limited = false;
	cookie.base    = str;
	cookie.i       = 0;

	emit.cookie = &cookie;
	emit.emit   = &__strappend_emit;

	len = __printf_impl(format, ap, emit);

	va_end(ap);

	return len;
}
