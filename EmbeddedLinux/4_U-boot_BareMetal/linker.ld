ENTRY(_start)

SECTIONS
{
    . = 0x80000;

    .text : {
        KEEP(*(.text.boot))
        *(.text*)
    }

    .rodata : { *(.rodata*) }
    .data   : { *(.data*)   }

    __bss_start = .;
    .bss    : { *(.bss*) *(COMMON) }
    __bss_end = .;
}