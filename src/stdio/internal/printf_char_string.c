#include <stdarg.h>
#include <string.h>

#include "printf_impl.h"
#include "stdio.h"
#include "stdlib.h"

void __printf_char_string(printf_conv_spec spec, printf_arg_value value,
			  printf_emit emit)
{
	char  *str;
	size_t pad;
	char   org_replaced_char;

	str = NULL;
	pad = 0;

	str = value.cp;

	if (spec.precision_flags & PRINTF_FLG_PREC_EXPL)
	{
		org_replaced_char   = str[spec.precision];
		str[spec.precision] = '\0';
	}

	if (spec.min_field_width > 0)
		pad = spec.min_field_width - strlen(str);

	if (spec.flags & PRINTF_FLG_LEFT_JUST)
	{
		__emit_string(emit, str);
		__printf_repeat(' ', pad, emit);
	}
	else
	{
		__printf_repeat(' ', pad, emit);
		__emit_string(emit, str);
	}

	if (spec.precision_flags & PRINTF_FLG_PREC_EXPL)
		str[spec.precision] = org_replaced_char;
}
