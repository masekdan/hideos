[bits 32]

    extern irq_handler

isr_common_stub:
    pushad

    push esp
    call irq_handler

    add esp, 4
    popad
    add esp, 8
    iretd

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

    global irq_ignore
irq_ignore:
    push 0
    push 0      ; Dummy čísla, aby seděl stack
    pushad
    ; Tady nemusíš volat nic, nebo jen pošli EOI
    mov al, 0x20
    out 0x20, al
    popad
    add esp, 8
    iretd