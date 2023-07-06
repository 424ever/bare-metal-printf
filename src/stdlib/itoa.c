#include <stdlib.h>

#define ITOA_BUF_SIZE 100

static char itoa_buf[ITOA_BUF_SIZE];

char *itoa(int i)
{
	char *itoa_curr;
	int   neg;

	if (i < 0)
		neg = 1;
	else
		neg = 0;

	itoa_curr  = itoa_buf + ITOA_BUF_SIZE - 1;
	*itoa_curr = '\0';

	if (i == 0)
	{
		--itoa_curr;
		*itoa_curr = '0';
		return itoa_curr;
	}

	while (i != 0)
	{
		--itoa_curr;
		*itoa_curr = '0' + i % 10;
		i	   = i / 10;
	}

	if (neg)
	{
		--itoa_curr;
		*itoa_curr = '-';
	}

	return itoa_curr;
}
