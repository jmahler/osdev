#ifndef _TERMINAL_H
#define _TERMINAL_H
 
#include <stdint.h>
#include <stddef.h>

#include "string.h"

/* Hardware text mode color constants. */
enum vga_color {
	COLOR_BLACK = 0,
	COLOR_BLUE = 1,
	COLOR_GREEN = 2,
	COLOR_CYAN = 3,
	COLOR_RED = 4,
	COLOR_MAGENTA = 5,
	COLOR_BROWN = 6,
	COLOR_LIGHT_GREY = 7,
	COLOR_DARK_GREY = 8,
	COLOR_LIGHT_BLUE = 9,
	COLOR_LIGHT_GREEN = 10,
	COLOR_LIGHT_CYAN = 11,
	COLOR_LIGHT_RED = 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_YELLOW = 14,
	COLOR_WHITE = 15,
};

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

uint16_t make_vgaentry(char c, enum vga_color fg, enum vga_color bg);

void terminal_putentryat(char c, enum vga_color fg, enum vga_color bg, size_t row, size_t col);

void terminal_initialize();

void terminal_putchar(char c);
void terminal_putcharc(char c, enum vga_color fg, enum vga_color bg);

void terminal_writestring(const char* data);

#endif /* _TERMINAL_H */
