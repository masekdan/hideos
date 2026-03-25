#include "serial.h"

void serial_init()
{
    outb(COM1 + 1, 0x00); // zakazat preruseni
    outb(COM1 + 1, 0x80); // povolit dlab

    outb(COM1 + 0, 0x03); // Low byte divisoru (3)
    outb(COM1 + 1, 0x00); // High byte divisoru (0)

    outb(COM1 + 3, 0x03); // 8 bitů, žádná parita, jeden stop bit (vypne DLAB)
    outb(COM1 + 2, 0xC7); // Povolit FIFO, vymazat je, 14-byte threshold
    outb(COM1 + 4, 0x0B); // IRQs povoleno, RTS/DSR nastaveno
}

int is_transmit_empty()
{
    return inb(COM1 + 5) & 0x20;
}

void serial_putchar(char c)
{
    while (is_transmit_empty() == 0);
    outb(COM1, c);
}

void serial_print(const char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        serial_putchar(str[i]);
        i++;
    }
}