# Parametry a názvy souborů
NASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

CFLAGS = -m32 -ffreestanding -fno-pic -fno-stack-protector -fno-asynchronous-unwind-tables -nostdlib
LDFLAGS = -m elf_i386 -T link.ld

# Cíle
all: image.bin

# Vytvoření finálního obrazu
image.bin: boot.bin kernel.bin
	cat boot.bin kernel.bin > image.bin
	truncate -s 32k image.bin

# Kompilace bootloaderu
boot.bin: fe-boot.asm
	$(NASM) -f bin fe-boot.asm -o boot.bin

# Kompilace jádra (převod z ELF do binárky)
kernel.bin: kernel.elf
	$(OBJCOPY) -O binary kernel.elf kernel.bin

# Linkování kernelu
kernel.elf: kernel.o
	$(LD) $(LDFLAGS) -o kernel.elf kernel.o

# Kompilace C souboru do objektu
kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

# Čištění vytvořených souborů
clean:
	rm -f *.o *.bin *.elf