#include "io.h"

void outb(unsigned short port, unsigned char c)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (c));
}
