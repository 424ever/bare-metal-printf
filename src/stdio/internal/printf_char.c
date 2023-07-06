#include <stdarg.h>

#include "printf_impl.h"

void _printf_char(bool numbered_args, va_list ap, printf_resolved_arg *args,
		  printf_conv_spec spec, emit_func emit, void *cookie)
{
	unsigned char c;

	if (!numbered_args)
		c = (unsigned char) va_arg(ap, int);
	else
		c = (unsigned char) args[spec.argnum].value.si;

	emit(cookie, c);
}
