#ifndef IO_PORTS_H
#define IO_PORTS_H


// gpu

#define VIDEO_WIDTH 80
#define VIDEO_HEIGHT 25
#define VIDEO_MEMORY (char *)0xb8000
#define DEFAULT_COLOR 0x0F // Bílá na černé

#define VGA_CTRL_REGISTER 0x3d4 // control register
#define VGA_DATA_REGISTER 0x3d5 // data register

#define VGA_CUR_LOW_REG 0x0f  // cursor location low register
#define VGA_CUR_HIGH_REG 0x0e // cursor location high register


// keyboard

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KEYBOARD_STATUS_OUTPUT_FULL 0x01

#define COM1 0x3F8 // serial lane

// ata

#define ATA_PRIMARY_DATA         0x1f0
#define ATA_PRIMARY_ERR          0x1f1
#define ATA_PRIMARY_SECCOUNT     0x1f2
#define ATA_PRIMARY_LBA_LOW      0x1f3
#define ATA_PRIMARY_LBA_MID      0x1f4
#define ATA_PRIMARY_LBA_HIGH     0x1f5
#define ATA_PRIMARY_DRIVE_SEL    0x1f6
#define ATA_PRIMARY_COMM_STAT    0x1f7

#define ATA_CMD_READ_PIO         0x20
#define ATA_CMD_WRITE_PIO        0x30

#endif