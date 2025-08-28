#!/bin/bash

rm out/mykernel.iso
rm -f out/*.o out/mykernel.elf
rm -f mykernel.elf
rm -f mykernel.iso

echo "Compiling kernel.c and start.s"
$HOME/opt/cross/bin/i686-elf-gcc -std=gnu99 -ffreestanding -g -c kernel/start.s -o out/start.o
$HOME/opt/cross/bin/i686-elf-gcc -std=gnu99 -ffreestanding -g -c kernel/kernel.c -o out/kernel.o

echo "Linking"
$HOME/opt/cross/bin/i686-elf-gcc -ffreestanding -nostdlib -g -T kernel/linker.ld out/start.o out/kernel.o -o mykernel.elf -lgcc

cp mykernel.elf iso/boot/kernel.bin
grub-mkrescue -o out/mykernel.iso iso

rm iso/boot/kernel.bin
rm -f out/*.o mykernel.elf