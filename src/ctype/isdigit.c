#include <ctype.h>

int isdigit(int c)
{
	c = (unsigned char) c;
	return c >= '0' && c <= '9';
}
