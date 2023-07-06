#include <string.h>

char *strchr(const char *s, int c)
{
	c = (unsigned char) c;

	if (!c)
		return (char *) s + strlen(s);

	for (; *s != '\0'; ++s)
	{
		if (*s == (unsigned char) c)
			return (char *) s;
	}

	return NULL;
}
