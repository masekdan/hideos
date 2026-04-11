#ifndef IDE_H
#define IDE_H

#include "arch/io_ports.h"
#include "arch/io.h"

void ata_read_sector(unsigned int lba, unsigned short* buffer);

#endif