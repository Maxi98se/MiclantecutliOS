#! /bin/bash

echo 'Compile boot.asm'
nasm -f bin ./boot.asm -o ./boot.bin

echo 'Compile kernel.asm'
nasm -f elf32 ./kernel.asm -o ./kernel.o

echo 'Compie kernel.c'
/home/maxi/opt/cross/bin/i386-elf-gcc -ffreestanding -mtune=i386 -mno-red-zone -c ./kernel.c -o gcc.o

echo 'Linking Files'
/home/maxi/opt/cross/bin/i386-elf-ld -Ttext 0x7e00 ./kernel.o ./gcc.o -o ./kernel.bin --oformat binary -m elf_i386

echo 'Making Image'
cat ./boot.bin ./kernel.bin > OS-0.1.img
