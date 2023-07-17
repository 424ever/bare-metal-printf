#include <string.h>

void *memset(void *s, int c, size_t n)
{
	size_t	       i;
	unsigned char *_s;

	_s = s;

	for (i = 0; i < n; ++i)
		_s[i] = (unsigned char) c;

	return s;
}
