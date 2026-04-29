[bits 16]
[org 0x7c00]

boot:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00

    mov [ BOOT_DRIVE ], dl

    ; DEBUG 1 – real mode
    ;mov ax, 0xb800
    ;mov es, ax
    ;mov di, 0
    ;mov byte [es:di], 'R'
    ;mov byte [es:di+1], 0x07


load_sector:
    mov ax, 0
    mov es, ax

    mov bx, 0x1000  ; memory where sectors will be placed kernel

    mov ah, 0x02    ; read desiderd sectors into memory
    mov al, 54      ; load 10 sectors
    mov ch, 0       ; cylinder 0
    mov cl, 0x02    ; CC SSSSSS sector = 2
    mov dl, [ BOOT_DRIVE ]    ; drive number = 0 default drive C
    mov dh, 0       ; CHS head = 0

    int 0x13        ; int 13H - fixed disc ops
    jc load_sector

    ; protected mode

    cli
    lgdt [ gdt_descriptor ]


    mov eax, cr0
    or eax, 1
    mov cr0, eax

    jmp CODE_SEG:protected_mode

gdt_start:
    dq 0x0

gdt_code:
    dw 0xffff
    dw 0
    db 0
    db 10011010b
    db 11001111b
    db 0

gdt_data:
    dw 0xffff
    dw 0
    db 0
    db 10010010b
    db 11001111b
    db 0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; Protected mode

[bits 32]

protected_mode:

    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax

    mov esp, 0x90000 ; stack

    ; DEBUG 2 – protected mode
    mov dword [0xb8000], 0x07200750    ; 'P ' (0x20) a 'P' (0x50) s atributem 0x07

    mov eax, 0x1000
    call eax

hang:
    jmp hang

BOOT_DRIVE db 0

times 510-($-$$) db 0
dw 0xaa55