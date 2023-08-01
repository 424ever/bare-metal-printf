#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "term.h"
#include "test.h"

static int failcount;

void printf_test(const char *name, const char *expected, const char *format,
		 ...)
{
	char	buffer[100];
	va_list ap;

	va_start(ap, format);
	vsnprintf(buffer, 100, format, ap);
	va_end(ap);

	if (strcmp(buffer, expected) == 0)
	{
		printf("'%s' passed\n", name);
	}
	else
	{
		++failcount;
		printf("'%s' failed\n  expected '%s'\n  got      '%s'\n", name,
		       expected, buffer);
	}
}

void _kernel_main(void)
{
	failcount = 0;

	run_tests();

	printf("=========\n%d tests failed", failcount);
}
