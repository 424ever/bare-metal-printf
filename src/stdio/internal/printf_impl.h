#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <wchar.h>

#ifndef __PRINTF_IMPL_H
#	define __PRINTF_IMPL_H

/* flags */
#	define PRINTF_FLAG_CHARS      "'-+ #0"
#	define PRINTF_FLG_TSND_GRP    0x01 /* Thousand's grouping "'" */
#	define PRINTF_FLG_LEFT_JUST   0x02 /* Left justified "-" */
#	define PRINTF_FLG_ALWAYS_SIGN 0x04 /* Always begin with a sign "+" */
#	define PRINTF_FLG_SPACE_SIGN  0x08 /* <space> if no sign char " " */
#	define PRINTF_FLG_HASH	       0x10 /* Alternative form "#" */
#	define PRINTF_FLG_ZERO	       0x20 /* Pad with zeros "0" */

/* flags for field width and precision */
#	define PRINTF_FLG_PREC_EXPL 0x01 /* Explicitly defined */
#	define PRINTF_FLG_PREC_ARG  0x02 /* Defined via int argument */

/* length modifiers */
#	define PTRINF_LEN_CHAR	       "hh" /* (un)signed char */
#	define PRINTF_LEN_SHORT       "h"  /* (unsigned) short */
#	define PRINTF_LEN_LONG	       "l"  /* (unsigned) long */
#	define PRINTF_LEN_LONG_LONG   "ll" /* (unsigned long long) */
#	define PRINTF_LEN_MAX	       "j"  /* (u)intmax_t */
#	define PRINTF_LEN_SIZE	       "z"  /* size_t */
#	define PRINTF_LEN_PTRDIFF     "t"  /* ptrdiff_t */
#	define PRINTF_LEN_LONG_DOUBLE "L"  /* long double */

/* conversion specifiers */
#	define PRINTF_CONV_SPEC_CHARS "diouxXfFeEgGaAcspnCS%"

/* type definitions */
typedef uint8_t printf_flags_t;
typedef uint8_t printf_prec_flags_t;
typedef char	printf_len_modifier_t[3];
typedef struct _printf_conv_spec
{
	int		      argnum;
	printf_flags_t	      flags;
	int		      min_field_width;
	printf_prec_flags_t   min_field_flags;
	int		      precision;
	printf_prec_flags_t   precision_flags;
	printf_len_modifier_t length_modifier;
	char		      conversion_specifier;
} printf_conv_spec;
typedef union _printf_arg_value
{
	char		  *cp;
	double		   d;
	int		   si;
	intmax_t	   sim;
	intmax_t	   simp;
	long		   sl;
	long		  *slp;
	long double	   ld;
	long long	   sll;
	long long	  *sllp;
	ptrdiff_t	   pd;
	ptrdiff_t	  *pdp;
	short		   ss;
	short		  *ssp;
	signed char	   sc;
	signed char	  *scp;
	size_t		   z;
	size_t		  *zp;
	uintmax_t	   uim;
	unsigned	   ui;
	unsigned char	   uc;
	unsigned long	   ul;
	unsigned long long ull;
	unsigned short	   us;
	void		  *vp;
	wchar_t		   wcp;
	wint_t		   wi;
} printf_arg_value;
typedef struct _printf_resolved_arg
{
	char		 type[3]; /* d, ld, c, s, ... */
	printf_arg_value value;
} printf_resolved_arg;
typedef void (*emit_func)(void *, char);

void emit_putchar(void *cookie, char c);
void emit_string(emit_func emit, void *cookie, const char *str);
int  printf_impl(const char *format, va_list ap, emit_func emit, void *cookie);

/* output functions */
void _printf_char(bool numbered_args, va_list ap, printf_resolved_arg *args,
		  printf_conv_spec spec, emit_func emit, void *cookie);
void _printf_char_string(bool numbered_args, va_list ap,
			 printf_resolved_arg *args, printf_conv_spec spec,
			 emit_func emit, void *cookie);
void _printf_unimplemented_specifier(char specifier, emit_func emit,
				     void *cookie);

#endif
