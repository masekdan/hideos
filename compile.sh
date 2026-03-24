#!/bin/bash

# 1. Kompilace bootloaderu
nasm -f bin fe-boot.asm -o boot.bin

# 2. Kompilace C kernelu
# Přidán flag -fno-asynchronous-unwind-tables pro čistší binárku
gcc -m32 -ffreestanding -fno-pic -fno-stack-protector -fno-asynchronous-unwind-tables -nostdlib -c kernel.c -o kernel.o

# 3. Linkování
ld -m elf_i386 -T link.ld -o kernel.elf kernel.o
objcopy -O binary kernel.elf kernel.bin

# 4. Spojení do obrazu disku
cat boot.bin kernel.bin > image.bin

# 5. Volitelné: Doplnění obrazu na pevnou velikost (např. 32KB), aby QEMU nehlásilo chyby čtení
truncate -s 32k image.bin

# clean
rm kernel.bin
rm kernel.elf
rm link.ld
rm kernel.o
rm boot.bin
