#ifndef SERIAL_H
#define SERIAL_H

#include "arch/io.h"
#include "arch/io_ports.h"

void serial_init();

int is_transmit_empty();

void serial_putchar(char c);

void serial_print(const char *str);

#endif