#include <stdarg.h>
#include <stdio.h>

#include "internal/printf_impl.h"

int vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
    sprintf_cookie cookie;
	printf_emit    emit;

    cookie.limited	 = true;
	cookie.remaining = size;
	cookie.base	 = str;
	cookie.i	 = 0;

	emit.cookie = &cookie;
	emit.emit   = &__strappend_emit;

	return __printf_impl(format, ap, emit);
}
