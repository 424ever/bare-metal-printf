#include <term.h>

uint8_t vga_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

void terminal_write(const char *data, size_t len)
{
	size_t i;

	for (i = 0; i < len; ++i)
		terminal_putchar(data[i]);
}

