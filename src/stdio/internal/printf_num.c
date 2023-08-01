#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "printf_impl.h"

#define __NUM_BUFFER_SIZE 100

static const char hex_digits[2][16] = {{'0', '1', '2', '3', '4', '5', '6', '7',
					'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'},
				       {'0', '1', '2', '3', '4', '5', '6', '7',
					'8', '9', 'A', 'B', 'C', 'D', 'E',
					'F'}};

static char num_buffer[__NUM_BUFFER_SIZE];

static void add_digits_with_maybe_thousands_grouping(char **end, int *numdigits,
						     printf_conv_spec spec,
						     int mod, int uppercase)
{
	--(*end);
	**end = hex_digits[uppercase][mod];
	++*numdigits;
	if (*numdigits % 3 == 0 && spec.flags & PRINTF_FLG_TSND_GRP)
	{
		--(*end);
		**end = ',';
	}
}

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
	pad				  = 0;

	if (base > 16)
		base = 16;

	if (n < 0)
		negative = true;

	if (n == 0 && spec.precision > 0)
	{
		--end;
		*end = '0';
		++numdigits;
	}
	for (; n != 0;)
	{
		mod = n % base;
		n /= 10;
		if (negative)
			mod = 16 - mod;
		add_digits_with_maybe_thousands_grouping(&end, &numdigits, spec,
							 mod, 0);
	}

	for (; numdigits < spec.precision; ++numdigits)
		*(--end) = '0';

	if (spec.min_field_width > 0)
		pad = spec.min_field_width - strlen(end) -
		      (negative || spec.flags & PRINTF_FLG_ALWAYS_SIGN ||
			       spec.flags & PRINTF_FLG_SPACE_SIGN
			   ? 1
			   : 0);
	if (pad < 0)
		pad = 0;

	if (spec.flags & PRINTF_FLG_LEFT_JUST)
		pad_left = false;

	if (spec.flags & PRINTF_FLG_ZERO)
	{
		sign_before_pad = true;
		if (!(spec.flags & PRINTF_FLG_LEFT_JUST))
			pad_char = '0';
	}

	if (pad_left)
	{
		if (sign_before_pad)
		{
			if (negative)
				emit.emit(emit.cookie, '-');
			else if (spec.flags & PRINTF_FLG_ALWAYS_SIGN)
				emit.emit(emit.cookie, '+');
			else if (spec.flags & PRINTF_FLG_SPACE_SIGN)
				emit.emit(emit.cookie, ' ');

			__printf_repeat(pad_char, pad, emit);
		}
		else
		{
			__printf_repeat(pad_char, pad, emit);

			if (negative)
				emit.emit(emit.cookie, '-');
			else if (spec.flags & PRINTF_FLG_ALWAYS_SIGN)
				emit.emit(emit.cookie, '+');
			else if (spec.flags & PRINTF_FLG_SPACE_SIGN)
				emit.emit(emit.cookie, ' ');
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

		__printf_repeat(pad_char, pad, emit);
	}
}
