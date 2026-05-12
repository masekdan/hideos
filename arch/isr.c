#include "isr.h"
#include "io.h"

void irq_handler(unsigned int irq_num)
{
    if (irq_num == 32)
    {
        // sched tick
    }
    else if (irq_num == 33)
    {
        // keyboard handler
    }

    if (irq_num >= 40) outb(0xA0, 0x20); // slave pic
    outb(0x20,0x20);                     // master pic
}
