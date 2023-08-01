#include <stdio.h>

#include "internal/printf_impl.h"

int sprintf(char *str, const char *format, ...)
{
	va_list ap;
	int	len;

	va_start(ap, format);

	len = vsprintf(str, format, ap);

	va_end(ap);

	return len;
}
