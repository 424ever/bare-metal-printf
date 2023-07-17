#include <stdio.h>

#include "stdio_internal.h"
#include "term.h"

int putchar(int c)
{
	__stdio_term_init();

	terminal_putchar((char) c);
	return (unsigned char) c;
}
