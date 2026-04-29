#ifndef IDE_H
#define IDE_H

#include "arch/io_ports.h"
#include "arch/io.h"

void ata_read_sector(unsigned char drive, unsigned int lba, unsigned short* buffer);

void ata_write_sector(unsigned char drive, unsigned int lba, unsigned short* buffer);

#endif