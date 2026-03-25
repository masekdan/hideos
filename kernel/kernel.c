#include "arch/io.h"
#include "arch/io_ports.h"

#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "drivers/serial.h"

#include "kernel/cli.h"

__attribute__((section(".text.entry"))) void kernel_main()
{

    vga_init();
    serial_init();

    vga_print("Welcome to HideOS\n");
    vga_print("LINE\n");
    vga_print("\n");
    vga_print("  _    _ _     _       ____   _____ \n");
    vga_print(" | |  | (_)   | |     / __ \\ / ____|\n");
    vga_print(" | |__| |_  __| | ___| |  | | (___  \n");
    vga_print(" |  __  | |/ _` |/ _ \\ |  | |\\___ \\ \n");
    vga_print(" | |  | | | (_| |  __/ |__| |____) |\n");
    vga_print(" |_|  |_|_|\\__,_|\\___|\\____/|_____/ \n");

    vga_print("\n\nPress any key to print:\n");

    unsigned char sc;
    char buffer[128];
    while (1)
    {
        sc = keyboard_get_char();
        if (sc != 0)
        {
            if (sc == 0x1C)
            {
                cli_readline(buffer,128);
            }
            else
            {
                vga_putchar(scancodes[sc]);
            }
        }
    }
}