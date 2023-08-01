#include <stdbool.h>
#include <stddef.h>
#include <string.h>

#include "stdlib.h"
#include "term.h"

#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_ORIGIN 0xB8000;

static int	 terminal_initialized = 0;
static int	 terminal_col;
static int	 terminal_row;
static uint16_t *terminal_buffer;
static uint8_t	 terminal_color;

static uint16_t vga_entry(unsigned char uc, uint8_t color)
{
	return (uint16_t) uc | (uint16_t) color << 8;
}

static void terminal_putentryat(char c, uint8_t color, int x, int y)
{
	terminal_buffer[y * VGA_WIDTH + x] = vga_entry(c, color);
}

static void terminal_scroll(void)
{
	size_t i;
	size_t row_len;

	row_len = VGA_WIDTH * sizeof(*terminal_buffer);

	memmove((uint8_t *) terminal_buffer, terminal_buffer + VGA_WIDTH,
		(VGA_HEIGHT - 1) * row_len);

	for (i = 0; i < VGA_WIDTH; ++i)
		terminal_putentryat(' ', terminal_color, i, VGA_HEIGHT - 1);

	terminal_col = 0;
	terminal_row = VGA_HEIGHT - 1;
}

static void terminal_newline(void)
{
	terminal_col = 0;
	if (++terminal_row == VGA_HEIGHT)
		terminal_scroll();
}

static void terminal_forward(void)
{
	if (++terminal_col == VGA_WIDTH)
		terminal_newline();
}

void terminal_initialize(void)
{
	int y;
	int x;

	terminal_row	= 0;
	terminal_col	= 0;
	terminal_color	= vga_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t *) VGA_ORIGIN;
	for (y = 0; y < VGA_HEIGHT; ++y)
	{
		for (x = 0; x < VGA_WIDTH; ++x)
			terminal_buffer[y * VGA_WIDTH + x] =
			    vga_entry(' ', terminal_color);
	}

	terminal_initialized = 1;
}

void terminal_setcolor(uint8_t color)
{
	terminal_color = color;
}

void terminal_putchar(char c)
{
	if (c == '\n')
		terminal_newline();
	else
	{
		terminal_putentryat(c, terminal_color, terminal_col,
				    terminal_row);
		terminal_forward();
	}
}

bool terminal_is_initialized(void)
{
	return terminal_initialized;
}
