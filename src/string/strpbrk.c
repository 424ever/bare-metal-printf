#include <string.h>

char *strpbrk(const char *s, const char *accept)
{
	char *candidate;
	char *tmp;

	candidate = NULL;

	for (; *accept != '\0'; ++accept)
	{
		tmp = strchr(s, *accept);
		if (tmp && (!candidate || tmp < candidate))
			candidate = tmp;
	}

	return candidate;
}
