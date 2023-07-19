#include <stddef.h>
#include <string.h>

#include "printf_impl.h"

void __printf_decimal(printf_conv_spec spec, printf_arg_value value,
		      printf_emit emit)
{
	intmax_t i;

	if (!strcmp(spec.length_modifier, PRINTF_LEN_CHAR))
		i = (signed char) value.si;
	else if (!strcmp(spec.length_modifier, PRINTF_LEN_SHORT))
		i = (short) value.si;
	else if (!strcmp(spec.length_modifier, PRINTF_LEN_LONG))
		i = value.sl;
	else if (!strcmp(spec.length_modifier, PRINTF_LEN_LONG_LONG))
		i = value.sll;
	else if (!strcmp(spec.length_modifier, PRINTF_LEN_MAX))
		i = value.sim;
	else if (!strcmp(spec.length_modifier, PRINTF_LEN_SIZE))
		i = value.z;
	else if (!strcmp(spec.length_modifier, PRINTF_LEN_PTRDIFF))
		i = value.pd;
	else
		i = value.si;

	__printf_print_num_s(i, 10, spec, emit);
}
