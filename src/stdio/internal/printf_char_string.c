#include <stdarg.h>

#include "printf_impl.h"

void _printf_char_string(bool numbered_args, va_list ap,
			 printf_resolved_arg *args, printf_conv_spec spec,
			 emit_func emit, void *cookie)
{
	char *str;

	if (!numbered_args)
		str = va_arg(ap, char *);
	else
		str = args[spec.argnum].value.cp;

	/* TODO: Handle min field with, precision, and flags */
	emit_string(emit, cookie, str);
}
