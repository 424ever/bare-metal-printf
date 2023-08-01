#include <stdio.h>

#include "internal/printf_impl.h"

int snprintf(char *str, size_t size, const char *format, ...)
{
	va_list ap;
	int	len;

	va_start(ap, format);

	len = vsnprintf(str, size, format, ap);

	va_end(ap);

	return len;
}
