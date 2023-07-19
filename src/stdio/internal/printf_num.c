#include <stdbool.h>
#include <string.h>

#include "printf_impl.h"

#define __NUM_BUFFER_SIZE 100

static const char hex_digits[2][16] = {{'0', '1', '2', '3', '4', '5', '6', '7',
					'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'},
				       {'0', '1', '2', '3', '4', '5', '6', '7',
					'8', '9', 'A', 'B', 'C', 'D', 'E',
					'F'}};

static char num_buffer[__NUM_BUFFER_SIZE];

void __printf_print_num_s(intmax_t n, int base, printf_conv_spec spec,
			  printf_emit emit)
{
	char *end;
	int   mod;
	int   pad;
	int   numdigits;
	bool  sign_before_pad;
	bool  negative;
	bool  pad_left;
	char  pad_char;

	num_buffer[__NUM_BUFFER_SIZE - 1] = '\0';
	end				  = num_buffer + __NUM_BUFFER_SIZE - 1;
	sign_before_pad			  = false;
	negative			  = false;
	pad_left			  = true;
	pad_char			  = ' ';
	numdigits			  = 0;

	if (base > 16)
		base = 16;

	if (n < 0)
		negative = true;

	for (; n != 0;)
	{
		mod = n % base;
		n /= 10;
		if (negative)
			mod = base - mod;
		--end;
		*end = hex_digits[0][mod];
		++numdigits;
	}

	for (; numdigits < spec.precision; ++numdigits)
		*(--end) = '0';

	pad = spec.min_field_width - strlen(end) -
	      (negative || spec.flags & PRINTF_FLG_ALWAYS_SIGN ? 1 : 0);

	if (spec.flags & PRINTF_FLG_LEFT_JUST)
		pad_left = false;

	if (spec.flags & PRINTF_FLG_ZERO)
	{
		sign_before_pad = true;
		pad_char	= '0';
	}

	if (pad_left)
	{
		if (sign_before_pad)
		{
			if (negative)
				emit.emit(emit.cookie, '-');
			else if (spec.flags & PRINTF_FLG_ALWAYS_SIGN)
				emit.emit(emit.cookie, '+');

			__printf_repeat(pad_char, pad, emit);
		}
		else
		{
			__printf_repeat(pad_char, pad, emit);

			if (negative)
				emit.emit(emit.cookie, '-');
			else if (spec.flags & PRINTF_FLG_ALWAYS_SIGN)
				emit.emit(emit.cookie, '+');
		}
		__emit_string(emit, end);
	}
	else
	{
		if (negative)
			emit.emit(emit.cookie, '-');
		else if (spec.flags & PRINTF_FLG_ALWAYS_SIGN)
			emit.emit(emit.cookie, '+');
		__emit_string(emit, end);
	}
}
