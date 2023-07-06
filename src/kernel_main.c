#include <stdio.h>
#include <string.h>

#include "stddef.h"
#include "stdlib.h"
#include "term.h"

void _kernel_main(void)
{
	printf("Hello, %s%c", "world", '!');
}
