#include <stdio.h>

#include "../stdio_internal.h"
#include "term.h"

void __stdio_term_init(void)
{
	if (terminal_is_initialized())
		return;

	terminal_initialize();
}
