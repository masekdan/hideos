#include "ide.h"

void ata_read_sector(unsigned int lba, unsigned short* buffer)
{
    outb(ATA_PRIMARY_DRIVE_SEL, 0xE0 | (lba >> 24) & 0x0F);


    outb(ATA_PRIMARY_SECCOUNT, 1);
    outb(ATA_PRIMARY_LBA_LOW, (unsigned char)lba);
    outb(ATA_PRIMARY_LBA_MID, (unsigned char)(lba >> 8));
    outb(ATA_PRIMARY_LBA_HIGH, (unsigned char)(lba >> 16));

    outb(ATA_PRIMARY_COMM_STAT, ATA_CMD_READ_PIO);

    while ((inb(ATA_PRIMARY_COMM_STAT) & 0x80));
    while (!(inb(ATA_PRIMARY_COMM_STAT) & 0x08));

    for (int i = 0; i<256;i++)
    {
        buffer[i] = inw(ATA_PRIMARY_DATA);
    }
}

void ata_write_sector(unsigned int lba, unsigned short* buffer)
{
    outb(ATA_PRIMARY_DRIVE_SEL, 0xE0 | ((lba >> 24) & 0x0F));

    outb(ATA_PRIMARY_SECCOUNT, 1);
    outb(ATA_PRIMARY_LBA_LOW,  (unsigned char)lba);
    outb(ATA_PRIMARY_LBA_MID,  (unsigned char)(lba >> 8));
    outb(ATA_PRIMARY_LBA_HIGH, (unsigned char)(lba >> 16));

    outb(ATA_PRIMARY_COMM_STAT, ATA_CMD_WRITE_PIO);

    while ((inb(ATA_PRIMARY_COMM_STAT) & 0x80)); 
    while (!(inb(ATA_PRIMARY_COMM_STAT) & 0x08)); 

    for (int i = 0; i < 256; i++) {
        outw(ATA_PRIMARY_DATA, buffer[i]);
    }

    outb(ATA_PRIMARY_COMM_STAT, 0xE7);
    while ((inb(ATA_PRIMARY_COMM_STAT) & 0x80));
}