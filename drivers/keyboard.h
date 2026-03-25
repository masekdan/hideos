#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "arch/io.h"
#include "arch/io_ports.h"

unsigned char keyboard_get_scancode();

unsigned char keyboard_get_char();

//void backspace();


extern char scancodes[128];


#endif