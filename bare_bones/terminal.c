
#include "terminal.h"

uint8_t make_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}
 
uint16_t make_vgaentry(char c, uint8_t color) {
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;
 
void terminal_initialize() {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
}
 
void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

void terminal_blank_line() {
	for (size_t col = terminal_column; col < VGA_WIDTH; col++) {
		const size_t index = terminal_row * VGA_WIDTH + col;
		terminal_buffer[index] = make_vgaentry(' ', terminal_color);
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
		terminal_putentryat(c, terminal_color, terminal_column,
							terminal_row);
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
