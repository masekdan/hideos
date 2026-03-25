# --- Nastavení nástrojů ---
NASM    = nasm
CC      = gcc
LD      = ld
OBJCOPY = objcopy

# --- Nastavení cest ---
SRCDIR = .
INCDIR = .  # Kořen pro include (umožní #include "drivers/vga.h")

# --- Detekce souborů ---
# Najde všechny .c soubory v celém stromu adresáře kernel/
SOURCES = $(shell find $(SRCDIR) -name '*.c')
# Vytvoří seznam .o souborů zachovávající cestu (např. kernel/drivers/vga.o)
OBJECTS = $(SOURCES:.c=.o)

# --- Kompilační flagy ---
# -m32: 32-bitový mód
# -ffreestanding: nepočítej se standardní knihovnou (no stdlib)
# -I$(INCDIR): cesta pro hledání hlavičkových souborů
CFLAGS = -m32 -ffreestanding -fno-pic -fno-stack-protector \
         -fno-asynchronous-unwind-tables -nostdlib -I$(INCDIR) -Wall -Wextra

# --- Linkovací flagy ---
LDFLAGS = -m elf_i386 -T link.ld

# --- Hlavní cíle ---

all: image.bin

# 1. Spojení bootloaderu a kernelu do finálního obrazu
image.bin: boot.bin kernel.bin
	@echo "Creating disk image..."
	cat boot.bin kernel.bin > image.bin
	truncate -s 32k image.bin

# 2. Kompilace bootloaderu (NASM)
boot.bin: fe-boot.asm
	@echo "Assembling bootloader..."
	$(NASM) -f bin fe-boot.asm -o boot.bin

# 3. Ořezání ELF souboru na čistou binárku
kernel.bin: kernel.elf
	@echo "Stripping ELF to binary..."
	$(OBJCOPY) -O binary kernel.elf kernel.bin

# 4. Linkování všech objektových souborů do jednoho ELF
kernel.elf: $(OBJECTS)
	@echo "Linking kernel..."
	$(LD) $(LDFLAGS) -o kernel.elf $(OBJECTS)

# 5. Generické pravidlo pro kompilaci .c souborů na .o
# % funguje jako wild-card (všimni si, že zachovává cestu k souboru)
%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# --- Úklid ---
clean:
	@echo "Cleaning up..."
	rm -f boot.bin kernel.bin kernel.elf image.bin
	find $(SRCDIR) -name '*.o' -delete

# Označení cílů, které nejsou soubory
.PHONY: all clean