#include <stdio.h>
#include <string.h>

int strcmp(const char *s1, const char *s2)
{
	for (; *s1 == *s2 && *s1; ++s1, ++s2)
		;

	if (*s1 < *s2)
		return -1;
	else if (*s2 < *s1)
		return 1;
	else
		return 0;
}
