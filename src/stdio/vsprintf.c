#include <stdarg.h>
#include <stdio.h>

#include "internal/printf_impl.h"

int vsprintf(char *str, const char *format, va_list ap)
{
    printf_emit emit;
    sprintf_cookie cookie;

    cookie.base = str;
    cookie.i    = 0;
    cookie.limited = false;
    
    emit.emit   = &__strappend_emit;
    emit.cookie = &cookie;

    return __printf_impl(format, ap, emit);
}
