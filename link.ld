/* link.ld */
ENTRY(kernel_main)

SECTIONS {
    . = 0x1000;

    .text : {
        /* Vynutíme, aby kernel_main byl na úplném začátku */
        *(.text.entry) 
        *(.text*)
    }

    .data : { *(.data*) }
    .bss  : { *(.bss*) *(COMMON) }

    /* Zahodíme nepotřebné sekce, které dělají neplechu */
    /DISCARD/ : {
        *(.note*)
        *(.comment*)
        *(.eh_frame)
    }
}