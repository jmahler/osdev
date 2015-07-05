
#include "terminal.h"

uint16_t make_vgaentry(char c, enum vga_color fg, enum vga_color bg, bool blink)
{
	uint16_t entry = 0;

	entry |= 0x00ff & c;
	entry |= 0x0f00 & fg << 8;
	entry |= 0x7000 & bg << 12;
	entry |= 0x8000 & blink << 15;

	return entry;
}

volatile uint16_t* terminal_buffer = (uint16_t*) 0xB8000;

void terminal_putentryat(char c, enum vga_color fg, enum vga_color bg, size_t row, size_t col) {
	size_t index = row * VGA_WIDTH + col;
	terminal_buffer[index] = make_vgaentry(c, fg, bg, 0);
}

enum vga_color terminal_bg;
enum vga_color terminal_fg;
 
void terminal_initialize() {
	terminal_bg = COLOR_BLACK;
	terminal_fg = COLOR_LIGHT_GREY;

	/* clear screen */
	for (size_t row = 0; row < VGA_HEIGHT; row++) {
		for (size_t col = 0; col < VGA_WIDTH; col++) {
			terminal_putentryat(' ', terminal_fg, terminal_bg, row, col);
		}
	}
}

static void terminal_scroll()
{
	/* shift all rows up one */
	for (size_t row = 1; row < VGA_HEIGHT; row++) {
		for (size_t col = 0; col < VGA_WIDTH; col++) {
			size_t src_idx = row * VGA_WIDTH + col;
			size_t dst_idx = (row - 1) * VGA_WIDTH + col;

			terminal_buffer[dst_idx] = terminal_buffer[src_idx];
		}
	}

	/* clear last row */
	size_t row = VGA_HEIGHT - 1;
	for (size_t col = 0; col < VGA_WIDTH; col++) {
		terminal_putentryat(' ', terminal_fg, terminal_bg, row, col);
	}
}

void terminal_putchar(char c) {
	terminal_putcharc(c, terminal_fg, terminal_bg);
}

void terminal_putcharc(char c, enum vga_color fg, enum vga_color bg) {

	static size_t terminal_row = 0;
	static size_t terminal_col = 0;

	if (c == '\n') {
		/* blank the rest of the line */
		for (; terminal_col < VGA_WIDTH; terminal_col++)
			terminal_putentryat(' ', fg, bg,
					terminal_row, terminal_col);
	} else {
		terminal_putentryat(c, fg, bg, terminal_row, terminal_col);
		terminal_col++;
	}

	if (terminal_col == VGA_WIDTH) {
		terminal_col = 0;
		terminal_row++;

		if (terminal_row == VGA_HEIGHT) {
			terminal_scroll();

			terminal_row = VGA_HEIGHT - 1;
		}
	}
}
 
void terminal_writestring(const char* data) {
	size_t datalen = strlen(data);
	for (size_t i = 0; i < datalen; i++)
		terminal_putchar(data[i]);
}
