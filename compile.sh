#! /bin/bash

echo 'Compile for:'
echo '(0) -> i386-elf'
echo '(1) -> i486-elf'

read -n1 -p ': ' arch
echo

case $arch in
  0)
    prefix=i386
    ;;
  1)
    prefix=i486
    ;;
  *)
    echo 'Wrong Input'
    exit 1
    ;;
esac

echo 'Compile boot.asm'
nasm -f bin ./boot.asm -o ./boot.bin

echo 'Compile kernel.asm'
nasm -f elf32 ./kernel.asm -o ./kernel.o

echo 'Compie kernel.c'
/home/maxi/opt/cross/bin/$prefix-elf-gcc -ffreestanding -mtune=$prefix -mno-red-zone -c ./kernel.c -o gcc.o

echo 'Linking Files'
/home/maxi/opt/cross/bin/$prefix-elf-ld -Ttext 0x7e00 ./kernel.o ./gcc.o -o ./kernel.bin --oformat binary -m elf_i386

echo 'Making Image'
cat ./boot.bin ./kernel.bin > OS-0.2.img
