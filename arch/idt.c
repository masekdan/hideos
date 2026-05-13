#include "idt.h"
#include "io.h"
#include "stdlib/string.h"


struct idt_entry_struct idt[256];

void pic_remap()
{
    outb(0x20, 0x11); // Start inicializace Master
    outb(0xA0, 0x11); // Start inicializace Slave
    outb(0x21, 0x20); // Master offset (32)
    outb(0xA1, 0x28); // Slave offset (40)
    outb(0x21, 0x04); // Master slave zapojení
    outb(0xA1, 0x02); 
    outb(0x21, 0x01); // Mode 8086
    outb(0xA1, 0x01);
    outb(0x21, 0x0);  // Povolit všechna přerušení (masky)
    outb(0xA1, 0x0);
    //outb(0x21,0xFD);
    //outb(0xA1,0xFF);
}


void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags)
{
    idt[num].offset_1 = (base & 0xFFFF);

    idt[num].selector = sel;

    idt[num].zero = 0;

    idt[num].type_attributes = flags;

    idt[num].offset_2 = (base >> 16) & 0xFFFF;
}

void idt_init()
{
    struct idt_ptr_struct idt_ptr;

    memset(&idt,0,sizeof(struct idt_entry_struct)*256);

    pic_remap();

    for(int i = 0; i < 256; i++) { // ignore ostarnich preruseni
        idt_set_gate(i, (unsigned int)irq_ignore, 0x08, 0x8e);
    }

    idt_set_gate(32, (unsigned int)irq0_wrapper, 0x08, 0x8e);
    idt_set_gate(33, (unsigned int)irq1_wrapper, 0x08, 0x8e);

    idt_ptr.limit = (sizeof(struct idt_entry_struct) * 256) -1;
    idt_ptr.base = (unsigned int)&idt;

    asm volatile("lidt (%0)" : : "r" (&idt_ptr));
}