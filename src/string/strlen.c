#include <string.h>

size_t strlen(s)
const char *s;
{
	size_t len = 0;

	while (*s != NULL)
	{
		++len;
		++s;
	}

	return len;
}

