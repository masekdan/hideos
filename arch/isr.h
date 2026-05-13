#ifndef ISR_h
#define ISR_H

typedef struct {
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int interupt, error;
    unsigned int eip, cs, eflags, useresp, ss;
} registers_t;

void irq_handler(registers_t regs);

#endif
