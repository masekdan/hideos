#include "arch/io.h"
#include "arch/io_ports.h"

#include "drivers/vga.h"
#include "drivers/keyboard.h"
#include "drivers/serial.h"

__attribute__((section(".text.entry")))
void kernel_main()
{

    __asm__ volatile ("mov $0x90000, %esp");

    vga_init();
    serial_init();
    //*((unsigned short*)0xB8000) = 0x2F4B; // Zelené 'K'

    while (1)
    {

    }
}