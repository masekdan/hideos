#ifndef GDT_H
#define GDT_H

struct gdt_entry_struct {
    unsigned short limit_low;    // Spodních 16 bitů limitu
    unsigned short base_low;     // Spodních 16 bitů adresy
    unsigned char  base_middle;  // Dalších 8 bitů adresy
    unsigned char  access;       // Přístupová práva
    unsigned char  granularity;
    unsigned char  base_high;    // Horních 8 bitů adresy
} __attribute__((packed));

struct gdt_ptr_struct {
    unsigned short limit;        // Velikost tabulky - 1
    unsigned int   base;         // Adresa prvního záznamu
} __attribute__((packed));

void gdt_set_gate(int num, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran);

void gdt_init();

extern void gdt_flush(unsigned int gdt_ptr);

#endif