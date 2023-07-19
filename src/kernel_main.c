#include <stdio.h>
#include <string.h>

#include "stddef.h"
#include "stdlib.h"
#include "term.h"

void _kernel_main(void)
{
	printf("Hello, %3$+7.5d%2$s%1$c%%", '!', "world", 42);
}
