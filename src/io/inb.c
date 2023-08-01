#include "io.h"

char inb(unsigned short port)
{
	char rc;
	asm volatile("inb %1, %0" : "=a"(rc) : "dN"(port));
	return rc;
}
