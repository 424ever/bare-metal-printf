#include <stdio.h>
#include <string.h>

#include "stdio_internal.h"
#include "term.h"

int puts(const char *s)
{
	size_t len;

	_stdio_term_init();

	len = strlen (s);
	terminal_write(s, len);

	return 1;
}

