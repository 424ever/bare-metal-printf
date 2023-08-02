#include <ctype.h>
#include <string.h>

#include "printf_impl.h"

void __printf_unsigned(int base, printf_conv_spec spec, printf_arg_value value,
		       printf_emit emit)
{
	uintmax_t i;
	bool	  uppercase;

	uppercase = false;

	if (!strcmp(spec.length_modifier, PRINTF_LEN_CHAR))
		i = (unsigned char) value.ui;
	else if (!strcmp(spec.length_modifier, PRINTF_LEN_SHORT))
		i = (unsigned short) value.ui;
	else if (!strcmp(spec.length_modifier, PRINTF_LEN_LONG))
		i = value.ul;
	else if (!strcmp(spec.length_modifier, PRINTF_LEN_LONG_LONG))
		i = value.ull;
	else if (!strcmp(spec.length_modifier, PRINTF_LEN_MAX))
		i = value.uim;
	else if (!strcmp(spec.length_modifier, PRINTF_LEN_SIZE))
		i = value.z;
	else if (!strcmp(spec.length_modifier, PRINTF_LEN_PTRDIFF))
		i = value.pd;
	else
		i = value.ui;

	if (isupper(spec.conversion_specifier))
		uppercase = true;

	__printf_print_num_u(i, base, spec, uppercase, emit);
}