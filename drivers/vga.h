#ifndef VGA_H
#define VGA_H

#include "arch/io.h"
#include "arch/io_ports.h"

int get_cursor();

void set_cursor(int offset);

void update_cursor(int x, int y);

void vga_clear_screen();

void vga_scroll(int offset);

void vga_putchar(char c);

void vga_print(char *str);

void vga_init();

extern int cursor_x, cursor_y;

#endif