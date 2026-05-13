#include "gdt.h"

struct gdt_entry_struct gdt[5];

struct gdt_ptr_struct ptr;

void gdt_set_gate(int num, unsigned int base, unsigned int limit, unsigned char access, unsigned char gran)
{
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access = access;
}

void gdt_init()
{
    ptr.limit = (sizeof(struct gdt_entry_struct) * 5) -1;
    ptr.base = (unsigned int)&gdt;

    gdt_set_gate(0,0,0,0,0);

    gdt_set_gate(1,0,0xFFFFFFFF,0x9A,0xCF);

    gdt_set_gate(2,0,0xFFFFFFFF,0x92,0xCF);

    gdt_flush((unsigned int)&ptr);
}
