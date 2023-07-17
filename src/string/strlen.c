#include <string.h>

size_t strlen(const char *s)
{
	size_t len;

	len = 0;

	while (*s != NULL)
	{
		++len;
		++s;
	}

	return len;
}
