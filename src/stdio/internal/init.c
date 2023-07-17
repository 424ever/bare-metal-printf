#include <stdio.h>

#include "../stdio_internal.h"
#include "term.h"

static int term_initialized = 0;

void __stdio_term_init(void)
{
	if (term_initialized)
		return;

	terminal_initialize();
	term_initialized = 1;
}
