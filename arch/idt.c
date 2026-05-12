#include "idt.h"
#include "io.h"


void pic_remap()
{
    outb(0x20, 0x11); // Start inicializace Master
    outb(0xA0, 0x11); // Start inicializace Slave
    outb(0x21, 0x20); // Master offset (32)
    outb(0xA1, 0x28); // Slave offset (40)
    outb(0x21, 0x04); // Master slave zapojení
    outb(0xA1, 0x02); 
    outb(0x21, 0x01); // Mode 8086
    outb(0xA1, 0x01);
    outb(0x21, 0x0);  // Povolit všechna přerušení (masky)
    outb(0xA1, 0x0);
}