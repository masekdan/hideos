#include "fat16.h"
#include "drivers/ide.h"
#include "stdlib/string.h"
#include "drivers/serial.h"
#include "drivers/vga.h"

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
    unsigned char sector_buffer[512];

    ata_read_sector(drive,0,sector_buffer);

    PartitionTable* pt = (PartitionTable*)((unsigned char*)sector_buffer + 0x1BE);

    partition_lba_start = pt[0].start_sector;

    ata_read_sector(drive,partition_lba_start,sector_buffer);

    memcpy(&bs, sector_buffer, sizeof(Fat16BootSector));
    
    fat_lba_start = partition_lba_start + bs.reserved_sectors;
    root_lba_start = fat_lba_start + (bs.number_of_fats * bs.fat_size_sectors);
    root_dir_sectors = (bs.root_dir_entries * 32 + (bs.sector_size - 1)) / bs.sector_size;
    data_lba_start = root_lba_start + root_dir_sectors;
    sectors_per_cluster = bs.sectors_per_cluster;
    current_dir_cluster = 0;

    serial_print("FAT16 Initialized\n");
    
}

void print_filename(Fat16Entry* entry) {
    char name[9]; // 8 znaků + nula
    char ext[4];  // 3 znaky + nula

    for(int i = 0; i < 8; i++) name[i] = entry->filename[i];
    name[8] = '\0';

    for(int i = 0; i < 3; i++) ext[i] = entry->ext[i];
    ext[3] = '\0';

    vga_print(name);
    vga_print(".");
    vga_print(ext);
}

void list_dir() {
    unsigned char buf[512];
    
    // Root adresář má v FAT16 fixní délku a umístění
    if (current_dir_cluster == 0) {
        for (unsigned int i = 0; i < root_dir_sectors; i++) {
            ata_read_sector(0, root_lba_start + i, buf);
            Fat16Entry* entries = (Fat16Entry*)buf;

            for (int j = 0; j < 16; j++) {
                if (entries[j].filename[0] == 0x00) return; // Konec adresáře
                if (entries[j].filename[0] == 0xE5) continue; // Smazaný soubor
                if (entries[j].attributes & 0x08) continue;   // Volume Label

                vga_print(" ");
                print_filename(&entries[j]);
                if (entries[j].attributes & 0x10) vga_print(" [DIR]");
                vga_print("\n");
            }
        }
    } else {
        vga_print("Listing subdirectories not implemented yet.\n");
    }
}


int change_dir(const char* name)
{
    Fat16Entry entry;

    if (strcmp(name,"..")==0)
    {
        if (current_dir_cluster == 0)
        {
            return 1;
        }
        return 0;
    }

    current_dir_cluster = entry.starting_cluster;
    return 1;
}