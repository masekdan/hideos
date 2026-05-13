#ifndef IDT_H
#define IDT_H

struct idt_entry_struct {
   unsigned short offset_1;        // offset bits 0..15
   unsigned short selector;        // a code segment selector in GDT or LDT
   unsigned char  zero;            // unused, set to 0
   unsigned char  type_attributes; // gate type, dpl, and p fields
   unsigned short offset_2;
} __attribute__((packed));

struct idt_ptr_struct {
    unsigned short limit;
    unsigned int base;
} __attribute__((packed));

extern void irq0_wrapper();
extern void irq1_wrapper();


void pic_remap();

void idt_set_gate(unsigned char num, unsigned int base, unsigned short sel, unsigned char flags);

void idt_init();

#endif
