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

    current_dir_cluster = 0; // we are in root

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

void list_dir()
{
    Fat16Entry entries[16];
    unsigned char sector_buffer[512];

    vga_print("\nListing directory\n");

    if (current_dir_cluster == 0)
    {
        for (unsigned int i =0 ; i<(bs.root_dir_entries*32/512);i++)
        {
            ata_read_sector(1, root_lba_start + i, sector_buffer);
            Fat16Entry* sector_entries = (Fat16Entry*)sector_buffer;

            for (int j = 0; j<16;j++)
            {
                Fat16Entry entry = sector_entries[j];


                if (entry.filename[0] == 0x00)
                {
                    serial_print("Done\n");
                    return;
                }
                if (entry.filename[0] == 0xE5) continue;
                if (entry.attributes == 0x0F) continue;

                print_filename(&entry);
                vga_print("\n");
            }
        }
        return;
    }

    unsigned short cluster = current_dir_cluster;
    while (cluster < 0xFFF8)
    {
        unsigned int cluster_lba = data_lba_start + (cluster - 2) * sectors_per_cluster;

        for (unsigned int s = 0; s<sectors_per_cluster; s++)
        {
            ata_read_sector(1, cluster_lba+s ,sector_buffer);
            Fat16Entry* sector_entries = (Fat16Entry*)sector_buffer;

            for (int j = 0; j<16;j++)
            {
                Fat16Entry entry = sector_entries[j];


                if (entry.filename[0] == 0x00)
                {
                    serial_print("Done\n");
                    return;
                }
                if (entry.filename[0] == 0xE5) continue;
                if (entry.attributes == 0x0F) continue;

                print_filename(&entry);
                vga_print("\n");
            }
        }
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