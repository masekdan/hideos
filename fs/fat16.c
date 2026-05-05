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
static unsigned short current_dir_cluster = 0; // se pak nastavi v init

void fat_init(unsigned char drive)
{
    unsigned char sector_buffer[512];

    ata_read_sector(drive, 0, sector_buffer);

    PartitionTable *pt = (PartitionTable *)((unsigned char *)sector_buffer + 0x1BE);

    partition_lba_start = pt[0].start_sector;

    ata_read_sector(drive, partition_lba_start, sector_buffer);

    memcpy(&bs, sector_buffer, sizeof(Fat16BootSector));

    fat_lba_start = partition_lba_start + bs.reserved_sectors;
    root_lba_start = fat_lba_start + (bs.number_of_fats * bs.fat_size_sectors);
    root_dir_sectors = (bs.root_dir_entries * 32 + (bs.sector_size - 1)) / bs.sector_size;
    data_lba_start = root_lba_start + root_dir_sectors;
    sectors_per_cluster = bs.sectors_per_cluster;
    current_dir_cluster = 0;

    serial_print("FAT16 Initialized\n");
}

void print_filename(Fat16Entry *entry)
{
    char name[9]; // 8 znaků + nula
    char ext[4];  // 3 znaky + nula

    for (int i = 0; i < 8; i++)
        name[i] = entry->filename[i];
    name[8] = '\0';

    for (int i = 0; i < 3; i++)
        ext[i] = entry->ext[i];
    ext[3] = '\0';

    vga_print(name);
    vga_print(".");
    vga_print(ext);
}

unsigned short get_fat_entry(unsigned int cluster)
{
    unsigned char buf[512];

    unsigned int fat_sector = fat_lba_start + (cluster / 256);
    unsigned int fat_offset = (cluster % 256) * 2;

    ata_read_sector(0, fat_sector, buf);
    return *(unsigned short *)&buf[fat_offset];
}

void split_filename(const char *fname, char out_name[8], char out_ext[3])
{
    // ..
    to_upper(fname);

    if (fname[0] == '.' && fname[1] == '.' && fname[2] == 0)
    {
        // name = "..      "
        out_name[0] = '.';
        out_name[1] = '.';
        for (int i = 2; i < 8; i++)
            out_name[i] = ' ';

        for (int i = 0; i < 3; i++)
            out_ext[i] = ' ';

        return;
    }

    int i = 0;
    int dot = -1;

    while (fname[i] != 0)
    {
        if (fname[i] == '.')
        {
            dot = i;
            break;
        }
        i++;
    }

    if (dot < 0)
    {
        for (i = 0; i < 8; i++)
            out_name[i] = ' ';
        for (i = 0; i < 3; i++)
            out_ext[i] = ' ';

        i = 0;
        while (fname[i] != 0 && i < 8)
        {
            out_name[i] = fname[i];
            i++;
        }
        return;
    }

    for (i = 0; i < 8; i++)
        out_name[i] = ' ';

    int j;
    for (j = 0; j < dot && j < 8; j++)
    {
        out_name[j] = fname[j];
    }

    for (i = 0; i < 3; i++)
        out_ext[i] = ' ';

    j = 0;
    i = dot + 1;
    while (fname[i] != 0 && j < 3)
    {
        out_ext[j] = fname[i];
        j++;
        i++;
    }
}


int find_file(const char *fname, Fat16Entry *out_entry)
{
    Fat16Entry entry;

    unsigned char buf[512];

    char name[8];
    char ext[3];
    split_filename(fname, name, ext);

    if (current_dir_cluster == 0)
    {
        for (unsigned i = 0; i < root_dir_sectors; i++)
        {
            ata_read_sector(0, root_lba_start, buf);
            Fat16Entry *entries = (Fat16Entry *)buf;
            for (int j = 0; j < 16; j++)
            {
                if (entries[j].filename[0] == 0x00)
                    return 0;
                if (entries[j].filename[0] == 0xE5)
                    continue;

                if ((strncmp((char *)entries[j].filename, name, 8)==0) && (strncmp((char *)entries[j].ext, ext, 3)==0))
                {
                    memcpy(out_entry, &entries[j], sizeof(Fat16Entry));
                    return 1;
                }
            }
        }
    }
    else
    {
        unsigned int cluster = current_dir_cluster;

        while (cluster < 0xFFF8)
        {
            unsigned int lba = data_lba_start + ((cluster - 2) * sectors_per_cluster);

            for (unsigned char s = 0; s < sectors_per_cluster; s++)
            {
                ata_read_sector(0, lba + s, buf);
                Fat16Entry *entries = (Fat16Entry *)buf;

                for (int j = 0; j < 16; j++)
                {
                    if (entries[j].filename[0] == 0x00)
                        return 0;
                    if (entries[j].filename[0] == 0xE5)
                        continue;

                    if ((strncmp((char *)entries[j].filename, name, 8)==0) && (strncmp((char *)entries[j].ext, ext, 3)==0))
                    {
                        memcpy(out_entry, &entries[j], sizeof(Fat16Entry));
                        return 1;
                    }
                }
            }

            cluster = get_fat_entry(cluster);

            if (cluster == 0 || cluster >= 0xFFF7)
                break;
        }
    }
    return 0;
}

void change_dir(const char *dir)
{
    Fat16Entry entry;
    if (find_file(dir, &entry) == 0)
    {
        vga_print("FNF\n");
    }
}

void list_dir()
{
    unsigned char buf[512];
    // Root adresář má v FAT16 fixní délku a umístění
    if (current_dir_cluster == 0)
    {
        for (unsigned int i = 0; i < root_dir_sectors; i++)
        {
            ata_read_sector(0, root_lba_start + i, buf);
            Fat16Entry *entries = (Fat16Entry *)buf;

            for (int j = 0; j < 16; j++)
            {
                if (entries[j].filename[0] == 0x00)
                    return; // Konec adresáře
                if (entries[j].filename[0] == 0xE5)
                    continue; // Smazaný soubor
                if (entries[j].attributes & 0x08)
                    continue; // Volume Label

                vga_print(" ");
                print_filename(&entries[j]);
                if (entries[j].attributes & 0x10)
                    vga_print(" [DIR]");
                vga_print("\n");
            }
        }
    }
    else
    {
        unsigned int cluster = current_dir_cluster;

        while (cluster < 0xFFF8)
        {
            unsigned int lba = data_lba_start + ((cluster - 2) * sectors_per_cluster);

            for (unsigned char s = 0; s < sectors_per_cluster; s++)
            {
                ata_read_sector(0, lba + s, buf);
                Fat16Entry *entries = (Fat16Entry *)buf;

                for (int j = 0; j < 16; j++)
                {
                    if (entries[j].filename[0] == 0x00)
                        return; // Konec adresáře
                    if (entries[j].filename[0] == 0xE5)
                        continue; // Smazaný soubor
                    if (entries[j].attributes & 0x08)
                        continue; // Volume Label

                    vga_print(" ");
                    print_filename(&entries[j]);
                    if (entries[j].attributes & 0x10)
                        vga_print(" [DIR]");
                    vga_print("\n");
                }
            }

            cluster = get_fat_entry(cluster);

            if (cluster == 0 || cluster >= 0xFFF7)
                break;
        }
    }
}