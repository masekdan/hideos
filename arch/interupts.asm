[bits 32]

    extern irq_handler

isr_common_stub:
    pusha
    call irq_handler
    popa
    iret

    global irq0_wrapper
irq0_wrapper:
    push 0
    push 32
    jmp isr_common_stub

    global irq1_wrapper
irq1_wrapper:
    push 0
    push 33
    jmp isr_common_stub