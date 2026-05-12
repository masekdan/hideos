#include "arch/io.h"
#include "arch/io_ports.h"

#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "drivers/serial.h"
#include "stdlib/string.h"

#include "shell/cli.h"
#include "fs/fat16.h"

int are_interrupts_enabled() {
    unsigned long flags;
    // Načte EFLAGS do proměnné flags
    asm volatile ( "pushfl\n\t"
                   "popl %0" : "=g"(flags) );
    return (flags & 0x200); // 0x200 je maska pro 9. bit (2^9)
}

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

    vga_print("\nType help to view command list:\n");

    fat_init(0);

    print_int(are_interrupts_enabled());

    cli_loop();

}