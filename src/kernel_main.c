#include <stdio.h>
#include <string.h>

#include "stddef.h"
#include "stdlib.h"
#include "term.h"

void _kernel_main(void)
{
	printf("Hello, '%7.3s''%*c'", "world", 30, '!');
}
