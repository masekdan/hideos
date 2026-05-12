[bits 32]

isr_common_stub:
    pusha
    call isr_handler
    popa
    iret