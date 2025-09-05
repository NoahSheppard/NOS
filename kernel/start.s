.extern kernel_main

.global start

.global gdt_flush
.extern _gp

.global idt_load
.extern idtp

.set MB_MAGIC, 0x1BADB002
.set MB_FLAGS, (1 << 0) | (1 << 1)
.set MB_CHECKSUM, (0 - (MB_MAGIC + MB_FLAGS))

.section .multiboot
    .align 4
    .long MB_MAGIC
    .long MB_FLAGS
    .long MB_CHECKSUM

.section .bss
    .align 16
    stack_bottom:
        .skip 4096
    stack_top:

.section .text
start:
    movl $stack_top, %esp
    call kernel_main
hang:
    cli
    hlt
    jmp hang

gdt_flush:
    lgdt _gp
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    ljmp $0x08, $reload_segments
reload_segments:
    ret

idt_load:
    lidt idtp
    ret