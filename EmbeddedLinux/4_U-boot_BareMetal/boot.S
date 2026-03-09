.section ".text.boot"
.global _start

_start:
    // park cores 1, 2, 3
    mrs     x0, mpidr_el1
    and     x0, x0, #0x3
    cbnz    x0, halt

    // set stack pointer to 0x80000
    ldr     x0, =_start
    mov     sp, x0

    // zero BSS
    ldr     x0, =__bss_start
    ldr     x1, =__bss_end
bss_loop:
    cmp     x0, x1
    b.ge    bss_done
    str     xzr, [x0], #8
    b       bss_loop
bss_done:

    bl      main

halt:
    wfe
    b halt