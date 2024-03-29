#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../stdio_internal.h"
#include "printf_impl.h"

/* global data */
static printf_resolved_arg resolved_numbered_args[NL_ARGMAX];
static bool		   printf_using_numbered_args;

/* function declaractions */
static bool determine_using_numbered_args(const char *format);
static int  parse_conv_spec(const char *str, printf_conv_spec *spec);
static int parse_width(const char *str, int *width, printf_prec_flags_t *flags);
static int printf_print_conv_spec(const char *format, va_list *ap,
				  printf_emit emit);
static void resolve_numbered_arg_values(const char *format, va_list *ap);
static printf_resolved_arg_type get_type_for(printf_conv_spec spec);
static void pop_arg_of_type(printf_resolved_arg_type type, va_list *ap,
			    printf_arg_value *value);

/* function definitions */
int __printf_impl(const char *format, va_list ap, printf_emit emit)
{
	int	len;
	va_list ap2;

	va_copy(ap2, ap);

	printf_using_numbered_args = determine_using_numbered_args(format);

	if (printf_using_numbered_args)
	{
		resolve_numbered_arg_values(format, &ap2);
	}

	len = 0;

	for (; *format != '\0'; ++format)
	{
		if (*format == '%')
		{
			format += printf_print_conv_spec(format, &ap2, emit);
		}
		else
			__emit_char(emit, *format);
	}

	va_end(ap2);

	return len;
}

void __printf_repeat(char c, size_t n, printf_emit emit)
{
	size_t i;

	for (i = 0; i < n; ++i)
		emit.emit(emit.cookie, c);
}

/*
 * DESCRIPTION
 *   Outputs a conversion specification _format_, starting at the initial %
 *   character using the passed emit function.
 * RETURN VALUE
 *   The number of characters in the conversion specification
 */
int printf_print_conv_spec(const char *format, va_list *ap, printf_emit emit)
{
	int			 len;
	printf_conv_spec	 spec;
	printf_arg_value	 value;
	printf_resolved_arg_type type;

	/* parse_conv_spec expects to start after the % */
	++format;

	len = parse_conv_spec(format, &spec) + 1;

	if (spec.min_field_flags & PRINTF_FLG_PREC_ARG)
	{
		if (printf_using_numbered_args)
			spec.min_field_width =
			    resolved_numbered_args[spec.min_field_width]
				.value.si;
		else
			spec.min_field_width = va_arg(*ap, int);

		if (spec.min_field_width < 0)
		{
			spec.flags |= PRINTF_FLG_LEFT_JUST;
			spec.min_field_width *= -1;
		}
	}

	if (spec.precision_flags & PRINTF_FLG_PREC_ARG)
	{
		if (printf_using_numbered_args)
			spec.precision =
			    resolved_numbered_args[spec.precision].value.si;
		else
			spec.precision = va_arg(*ap, int);

		if (spec.precision < 0)
			spec.precision = 0;
	}

	if (printf_using_numbered_args)
		value = resolved_numbered_args[spec.argnum].value;
	else
	{
		type = get_type_for(spec);
		pop_arg_of_type(type, ap, &value);
	}

	switch (spec.conversion_specifier)
	{
	case 'd':
	case 'i':
		__printf_decimal(spec, value, emit);
		break;
	case 'c':
		__printf_char(spec, value, emit);
		break;
	case 's':
		__printf_char_string(spec, value, emit);
		break;
	case 'o':
		__printf_unsigned(8, spec, value, emit);
		break;
	case 'u':
		__printf_unsigned(10, spec, value, emit);
		break;
	case 'x':
	case 'X':
		__printf_unsigned(16, spec, value, emit);
		break;
	case '%':
		__emit_char(emit, '%');
		break;
	default:
		__printf_unimplemented_specifier(spec.conversion_specifier,
						 emit);
	}

	return len;
}

/*
 * DESCRIPTION
 *   Determines if the given format uses numbered arguments or relies of
 * implicit order.
 * RETURN VALUE
 *   1 if the format uses numbered arguments 0 if the
 *   format uses implicit argument ordering, or there are no format specifiers
 */
bool determine_using_numbered_args(const char *format)
{
	format = strchr(format, '%');

	if (!format)
		return false;

	/*
	 * A call to printf() and friends can have either only implicit order
	 * *or* numbered arguments, so it should be enough to only check the
	 * first format specifier.
	 */
	++format;
	for (; isdigit(*format); ++format)
		;
	if (*format == '$')
		return true;

	return false;
}

printf_resolved_arg_type get_type_for(printf_conv_spec spec)
{
	printf_resolved_arg_type type;

	type = INT;
	if (strchr("sn", spec.conversion_specifier))
		type = POINTER;

	else
	{
		if (!strcmp(spec.length_modifier, PRINTF_LEN_CHAR))
			type = CHAR;

		if (!strcmp(spec.length_modifier, PRINTF_LEN_SHORT))
			type = SHORT;

		if (!strcmp(spec.length_modifier, PRINTF_LEN_LONG))
			type = LONG;

		if (!strcmp(spec.length_modifier, PRINTF_LEN_LONG_LONG))
			type = LONGLONG;

		if (!strcmp(spec.length_modifier, PRINTF_LEN_MAX))
			type = INTMAX;

		if (!strcmp(spec.length_modifier, PRINTF_LEN_SIZE))
			type = SIZET;

		if (!strcmp(spec.length_modifier, PRINTF_LEN_PTRDIFF))
			type = PTRDIFF;

		if (strchr("fFeEgGaA", spec.conversion_specifier))
			type = DOUBLE;

		if (!strcmp(spec.length_modifier, PRINTF_LEN_LONG_DOUBLE))
			type = LONGDOUBLE;
	}
	return type;
}

void pop_arg_of_type(printf_resolved_arg_type type, va_list *ap,
		     printf_arg_value *value)
{
	switch (type)
	{
	case INT:
		value->si = va_arg(*ap, int);
		break;
	case DOUBLE:
		value->d = va_arg(*ap, double);
		break;
	case LONGDOUBLE:
		value->ld = va_arg(*ap, long double);
		break;
	case CHAR:
		value->sc = (signed char) va_arg(*ap, int);
		break;
	case SHORT:
		value->ss = (short) va_arg(*ap, int);
		break;
	case LONG:
		value->sl = va_arg(*ap, long);
		break;
	case LONGLONG:
		value->sll = va_arg(*ap, long long);
		break;
	case INTMAX:
		value->sim = va_arg(*ap, intmax_t);
		break;
	case PTRDIFF:
		value->pd = va_arg(*ap, ptrdiff_t);
		break;
	case SIZET:
		value->z = va_arg(*ap, size_t);
		break;
	case POINTER:
		value->vp = va_arg(*ap, void *);
		break;
	}
}

/*
 * DESCRIPTION
 *   Resolves the values of numbered arguments
 * RETURN VALUE
 *   None
 */
void resolve_numbered_arg_values(const char *format, va_list *ap)
{
	size_t		     i;
	printf_conv_spec     spec;
	printf_resolved_arg *arg;

	memset(resolved_numbered_args, 0,
	       NL_ARGMAX * sizeof(*resolved_numbered_args));

	for (; (format = strchr(format, '%'));)
	{
		++format;
		parse_conv_spec(format, &spec);

		if (spec.conversion_specifier == '%')
			continue;

		arg = resolved_numbered_args + spec.argnum;

		arg->used = true;

		arg->type = get_type_for(spec);
	}

	for (i = 0; i < NL_ARGMAX; ++i)
	{
		arg = resolved_numbered_args + i;
		if (!arg->used)
			break;

		pop_arg_of_type(arg->type, ap, &arg->value);
	}
}

/*
 * DESCRIPTION
 *   Parses a conversion specification from a string, starting after the '%'
 *   character. If an error occures, the contents of *spec are undefined.
 * RETURN VAUE
 *   Number of characters used or -1 on error
 */
int parse_conv_spec(const char *str, printf_conv_spec *spec)
{
	int   len;
	int   i;
	char *srch;

	memset(spec, 0, sizeof(*spec));

	if (!(srch = strpbrk(str, PRINTF_CONV_SPEC_CHARS)))
		return -1;

	len			   = srch - str;
	spec->conversion_specifier = *srch;

	srch = strchr(str, '$');

	if (srch && (srch - str) < len)
	{
		/* parse explicit argument number */
		for (; isdigit(*str); ++str)
		{
			spec->argnum *= 10;
			spec->argnum += *str - '0';
		}

		/*
		 * Make argnum 0 based and fit it into range
		 * [0,{NL_ARGMAX}-1]. Posix explicitly says that
		 * providing a higher number than NL_ARGMAX is undefined
		 * behavior, and this ensures we don't read out of
		 * bounds.
		 */
		--spec->argnum;
		spec->argnum %= NL_ARGMAX;
		++str;
	}

	/* parse flags */
	for (; (srch = strchr(PRINTF_FLAG_CHARS, *str)); ++str)
		spec->flags |= 1 << (srch - PRINTF_FLAG_CHARS);

	/* [...] if the <space> and '+' flags both appear, the <space> flag
	 * shall be ignored. */
	if ((spec->flags & PRINTF_FLG_SPACE_SIGN) &&
	    (spec->flags & PRINTF_FLG_ALWAYS_SIGN))
		spec->flags &= ~PRINTF_FLG_SPACE_SIGN;

	/* parse min field width */
	str += parse_width(str, &spec->min_field_width, &spec->min_field_flags);

	/* parse precision */
	if (*str == '.')
	{
		++str;
		str +=
		    parse_width(str, &spec->precision, &spec->precision_flags);
	}

	/* parse length modifier */
	srch = strpbrk(str, PRINTF_CONV_SPEC_CHARS);
	for (i = 0; i < 2 && str + i != srch; ++i)
		spec->length_modifier[i] = str[i];
	spec->length_modifier[i] = '\0';
	str += i;

	/* special wide cases for 's' and 'c' */
	if (spec->conversion_specifier == 'S')
	{
		spec->conversion_specifier = 's';
		spec->length_modifier[0]   = 'l';
		spec->length_modifier[1]   = '\0';
	}
	if (spec->conversion_specifier == 'C')
	{
		spec->conversion_specifier = 'c';
		spec->length_modifier[0]   = 'l';
		spec->length_modifier[1]   = '\0';
	}

	/* For d, i, o, u, x, and X conversion specifiers, if a precision is
	 * specified, the '0' flag shall be ignored. */
	if (strchr("diouxX", spec->conversion_specifier) != NULL &&
	    spec->precision_flags & PRINTF_FLG_PREC_EXPL)
		spec->flags &= ~PRINTF_FLG_ZERO;

	return len;
}

/*
 * DESCRIPTION
 *   Parses a minimum field width or precision from a string
 * RETURN VAUE
 *   Number of characters used.
 */
int parse_width(const char *str, int *width, printf_prec_flags_t *flags)
{
	int len;

	*width = 0;
	*flags = 0;
	len    = 0;

	if (!(isdigit(*str) || *str == '*'))
		return 0;

	*flags |= PRINTF_FLG_PREC_EXPL;
	if (*str == '*')
	{
		*flags |= PRINTF_FLG_PREC_ARG;
		++str;
		++len;
	}

	for (; isdigit(*str); ++str)
	{
		++len;
		*width *= 10;
		*width += *str - '0';
	}

	/* Skip $ char if indirect */
	if (*flags & PRINTF_FLG_PREC_ARG && *str == '$')
	{
		--*width;
		*width %= NL_ARGMAX;
		++len;
	}

	return len;
}

#ifdef DEBUG
void __debug_print_conv_spec(printf_conv_spec spec)
{
	puts("SPEC\n");

	puts("  argnum: ");
	puts(itoa(spec.argnum));
	putchar('\n');
	puts("  flags: ");
	if (spec.flags & PRINTF_FLG_TSND_GRP)
		puts("' ");
	if (spec.flags & PRINTF_FLG_LEFT_JUST)
		puts("- ");
	if (spec.flags & PRINTF_FLG_ALWAYS_SIGN)
		puts("+ ");
	if (spec.flags & PRINTF_FLG_SPACE_SIGN)
		puts("<space> ");
	if (spec.flags & PRINTF_FLG_HASH)
		puts("# ");
	if (spec.flags & PRINTF_FLG_ZERO)
		puts("0 ");
	putchar('\n');

	puts("  min field width: ");
	puts(itoa(spec.min_field_width));
	puts("( ");
	if (spec.min_field_flags & PRINTF_FLG_PREC_EXPL)
		puts("explicit ");
	if (spec.min_field_flags & PRINTF_FLG_PREC_ARG)
		puts("argument ");
	puts(" )\n");

	puts("  precision: ");
	puts(itoa(spec.precision));
	puts("( ");
	if (spec.precision_flags & PRINTF_FLG_PREC_EXPL)
		puts("explicit ");
	if (spec.precision_flags & PRINTF_FLG_PREC_ARG)
		puts("argument ");
	puts(")\n");

	puts("  length modifier: ");
	puts(spec.length_modifier);
	putchar('\n');

	puts("  conversion specifier: '");
	putchar(spec.conversion_specifier);
	puts("'\n");
}
#else
void __debug_print_conv_spec(printf_conv_spec spec)
{
	(void) spec;
}
#endif

#ifdef DEBUG
void __debug_print_resolved_args(void)
{
	size_t i;

	for (i = 0; i < NL_ARGMAX; ++i)
	{
		if (!resolved_numbered_args[i].used)
			continue;
		puts("ARG\n");
		puts("  num :");
		puts(itoa(i));
		puts("  type: ");
		puts(itoa(resolved_numbered_args[i].type));
		puts("\n");
	}
}
#else
void __debug_print_resolved_args(void)
{
}
#endif
