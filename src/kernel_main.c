#include <stdio.h>
#include <string.h>

#include "stddef.h"
#include "stdlib.h"
#include "term.h"

void _kernel_main(void)
{
	char test[100];
	snprintf(test, 100, "%98cabcd", '@');
	puts(test);
}
