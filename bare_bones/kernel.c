
#include <stddef.h>
#include <stdint.h>
 
#include "string.h"
#include "terminal.h"

/* Check if the compiler thinks if we are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif
 
/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif
 
void kernel_main() {
	/* Initialize terminal interface */
	terminal_initialize();
 
	terminal_writestring("Hello, kernel World!\n");

	for (char c = 0; c <= 15; c++) {
		terminal_putcharc('X', c, c);
	}
	terminal_putchar('\n');
}
