#include "fat16.h"
#include "drivers/ide.h"

// Všechny tyto offsety jsou nyní v SEKTORECH (LBA)
static unsigned int partition_lba_start = 0;
static unsigned int fat_lba_start = 0;
static unsigned int root_lba_start = 0;
static unsigned int data_lba_start = 0;

static unsigned int root_dir_sectors = 0;
static unsigned int sectors_per_cluster = 0;

static Fat16BootSector bs;
static unsigned short current_dir_cluster = 0;

void fat_init(unsigned char drive)
{
    unsigned short sector_buffer[256];

    ata_read_sector(0,sector_buffer);
}