#include <stdbool.h>

#include "io.h"
#include "term.h"

#define COM1 0x3F8

static bool terminal_initialized = 0;

static int is_transmit_empty()
{
	return inb(COM1 + 5) & 0x20;
}

bool terminal_is_initialized(void)
{
	return terminal_initialized;
}

void terminal_initialize(void)
{
	outb(COM1 + 1, 0x00); 
	outb(COM1 + 3, 0x80); 
	outb(COM1 + 0, 0x03); 
	outb(COM1 + 1, 0x00); 
	outb(COM1 + 3, 0x03); 
	outb(COM1 + 2, 0xC7); 
	outb(COM1 + 4, 0x0B); 
}

void terminal_setcolor(uint8_t color)
{
	/* no */
	(void) color;
}

void terminal_putchar(char c)
{
	while (!is_transmit_empty())
		;
	outb(COM1, c);
}
