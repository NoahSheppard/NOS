#!/bin/bash

rm out/mykernel.iso
rm -f out/*.o mykernel.elf
rm -f mykernel.elf
rm -f mykernel.iso

echo "Compiling kernel.c and start.s"
$HOME/opt/cross/bin/i686-elf-gcc -std=gnu99 -ffreestanding -g -c kernel/start.s -o out/start.o
$HOME/opt/cross/bin/i686-elf-gcc -std=gnu99 -ffreestanding -g -c kernel/kernel.c -o out/kernel.o
$HOME/opt/cross/bin/i686-elf-gcc -std=gnu99 -ffreestanding -g -c kernel/gdt.c -o out/gdt.o
$HOME/opt/cross/bin/i686-elf-gcc -std=gnu99 -ffreestanding -g -c kernel/idt.c -o out/idt.o
$HOME/opt/cross/bin/i686-elf-gcc -std=gnu99 -ffreestanding -g -c kernel/isrs.c -o out/isrs.o
$HOME/opt/cross/bin/i686-elf-gcc -std=gnu99 -ffreestanding -g -c kernel/irq.c -o out/irq.o
$HOME/opt/cross/bin/i686-elf-gcc -std=gnu99 -ffreestanding -g -c kernel/timer.c -o out/timer.o
$HOME/opt/cross/bin/i686-elf-gcc -std=gnu99 -ffreestanding -g -c kernel/kb.c -o out/kb.o
$HOME/opt/cross/bin/i686-elf-gcc -std=gnu99 -ffreestanding -g -c kernel/basicterm.c -o out/basicterm.o
$HOME/opt/cross/bin/i686-elf-gcc -std=gnu99 -ffreestanding -g -c kernel/libc/utils.c -o out/utils.o

echo "Linking"
$HOME/opt/cross/bin/i686-elf-gcc -ffreestanding -nostdlib -g -T kernel/linker.ld \
    out/start.o \
    out/kernel.o \
    out/gdt.o \
    out/idt.o \
    out/isrs.o \
    out/irq.o \
    out/timer.o \
    out/kb.o \
    out/basicterm.o \
    out/utils.o \
    -o mykernel.elf -lgcc

cp mykernel.elf iso/boot/kernel.bin
grub-mkrescue -o out/mykernel.iso iso

rm iso/boot/kernel.bin
 rm -f out/*.o mykernel.elf