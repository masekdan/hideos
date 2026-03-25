#ifndef IO_PORTS_H
#define IO_PORTS_H

#define VIDEO_WIDTH 80
#define VIDEO_HEIGHT 25
#define VIDEO_MEMORY (char *)0xb8000
#define DEFAULT_COLOR 0x0F // Bílá na černé

#define VGA_CTRL_REGISTER 0x3d4 // control register
#define VGA_DATA_REGISTER 0x3d5 // data register

#define VGA_CUR_LOW_REG 0x0f  // cursor location low register
#define VGA_CUR_HIGH_REG 0x0e // cursor location high register

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_STATUS_OUTPUT_FULL 0x01

#define COM1 0x3F8 // serial lane


#endif