#include <stdio.h>

#include "internal/printf_impl.h"

int snprintf(char *str, size_t size, const char *format, ...)
{
	sprintf_cookie cookie;
	va_list	       ap;
	printf_emit    emit;
	int	       len;

	va_start(ap, format);

	cookie.limited	 = true;
	cookie.remaining = size;
	cookie.base	 = str;
	cookie.i	 = 0;

	emit.cookie = &cookie;
	emit.emit   = &__strappend_emit;

	len = __printf_impl(format, ap, emit);

	va_end(ap);

	return len;
}
