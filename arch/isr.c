#include "isr.h"
#include "io.h"

void irq_handler(registers_t* regs)
{
    if (regs->interupt == 32)
    {
        // sched tick
    }
    else if (regs->interupt == 33)
    {
        // keyboard handler
    }

    if (regs->interupt >= 40) outb(0xA0, 0x20); // slave pic
    outb(0x20,0x20);                     // master pic
}
