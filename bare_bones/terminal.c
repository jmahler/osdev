
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

size_t terminal_row;
size_t terminal_column;
enum vga_color terminal_bg;
enum vga_color terminal_fg;
uint16_t* terminal_buffer;
 
void terminal_initialize() {
	terminal_row = 0;
	terminal_column = 0;
	terminal_bg = COLOR_BLACK;
	terminal_fg = COLOR_LIGHT_GREY;
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_fg, terminal_bg, 0);
		}
	}
}

void terminal_putentryat(char c, enum vga_color fg, enum vga_color bg, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, fg, bg, 0);
}

void terminal_blank_line() {
	for (size_t col = terminal_column; col < VGA_WIDTH; col++) {
		const size_t index = terminal_row * VGA_WIDTH + col;
		terminal_buffer[index] = make_vgaentry(' ', terminal_fg, terminal_bg, 0);
	}
}

void terminal_scroll() {

	for (size_t row = 1; row < VGA_HEIGHT; row++) {
		for (size_t col = 0; col < VGA_WIDTH; col++) {
			size_t src_idx = row * VGA_WIDTH + col;
			size_t dst_idx = (row - 1) * VGA_WIDTH + col;

			terminal_buffer[dst_idx] = terminal_buffer[src_idx];
		}
	}

	terminal_row = VGA_HEIGHT - 1;
	terminal_blank_line();
}

void terminal_putchar(char c) {
	if (c == '\n') {
		terminal_blank_line();
		terminal_column = VGA_WIDTH;
	} else {
		terminal_putentryat(c, terminal_fg, terminal_bg,
					terminal_column, terminal_row);
		terminal_column++;
	}

	if (terminal_column == VGA_WIDTH) {
		terminal_column = 0;

		if (++terminal_row == VGA_HEIGHT) {
			terminal_row = VGA_HEIGHT - 1;
			terminal_scroll();
		}
	}
}
 
void terminal_writestring(const char* data) {
	size_t datalen = strlen(data);
	for (size_t i = 0; i < datalen; i++)
		terminal_putchar(data[i]);
}
