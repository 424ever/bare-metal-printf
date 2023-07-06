#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void memcpy_internal(void *dest, const void *src, size_t n,
			    int back2front)
{
	const unsigned char *_src;
	size_t		     i;
	unsigned char	    *_dest;

	_dest = dest;
	_src  = src;

	if (back2front)
	{
		for (i = 1; i <= n; ++i)
			_dest[n - i] = _src[n - i];
	}
	else
	{
		for (i = 0; i < n; ++i)
			_dest[i] = _src[i];
	}
}

void *memcpy(void *dest, const void *src, size_t n)
{
	memcpy_internal(dest, src, n, 0);

	return dest;
}

void *memmove(void *dest, const void *src, size_t n)
{
	/*
	 * If `src` is after `dest` in memory, copy from front to back.
	 */
	if (src > dest)
		memcpy_internal(dest, src, n, 0);

	/*
	 * Otherwise, copy from back to front.
	 */
	else
		memcpy_internal(dest, src, n, 1);

	return dest;
}
