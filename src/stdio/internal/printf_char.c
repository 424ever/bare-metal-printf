#include <stdarg.h>
#include <stdio.h>

#include "printf_impl.h"
#include "stdlib.h"

void __printf_char(printf_conv_spec spec, printf_arg_value value,
		   printf_emit emit)
{
	unsigned char c;
	size_t	      pad;

	c   = 0;
	pad = 0;

	c = (unsigned char) value.si;

	if (spec.min_field_width > 0)
		pad = spec.min_field_width - 1;

	if (spec.flags & PRINTF_FLG_LEFT_JUST)
	{
		__emit_char(emit, c);
		__printf_repeat(' ', pad, emit);
	}
	else
	{
		__printf_repeat(' ', pad, emit);
		__emit_char(emit, c);
	}
}
