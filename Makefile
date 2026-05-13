# --- Nastavení nástrojů ---
NASM    = nasm
CC      = gcc
LD      = ld
OBJCOPY = objcopy

# --- Soubory a obrazy ---
FS_IMAGE    = fat16.img
FINAL_IMAGE = sd.img
BOOT_BIN    = boot.bin
KERNEL_BIN  = kernel.bin
KERNEL_ELF  = kernel.elf

# --- Nastavení cest ---
SRCDIR = .
INCDIR = .

# --- Detekce zdrojových souborů ---
# C soubory
SOURCES_C   = $(shell find $(SRCDIR) -name '*.c')
OBJECTS_C   = $(SOURCES_C:.c=.o)

# ASM soubory (pouze ty pro kernel, vynecháme bootloader)
SOURCES_ASM = $(shell find $(SRCDIR) -name '*.asm' ! -name 'fe-boot.asm')
OBJECTS_ASM = $(SOURCES_ASM:.asm=.o)

# Všechny objekty dohromady pro linker
ALL_OBJECTS = $(OBJECTS_C) $(OBJECTS_ASM)

# --- Kompilační flagy ---
CFLAGS = -m32 -ffreestanding -fno-pic -fno-stack-protector \
         -fno-asynchronous-unwind-tables -nostdlib -I$(INCDIR) -Wall -Wextra

# --- Linkovací flagy ---
LDFLAGS = -m elf_i386 -T link.ld

# --- Hlavní cíle ---

all: $(FINAL_IMAGE)

# 1. Tvorba finálního SD obrazu
$(FINAL_IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	@echo "--- Creating 32MB raw disk image with MBR ---"
	dd if=/dev/zero of=$(FINAL_IMAGE) bs=1M count=32
	@echo "2048,,6,*" | sfdisk $(FINAL_IMAGE) > /dev/null 2>&1
	dd if=$(BOOT_BIN) of=$(FINAL_IMAGE) bs=446 count=1 conv=notrunc
	dd if=$(BOOT_BIN) of=$(FINAL_IMAGE) bs=1 skip=510 seek=510 count=2 conv=notrunc
	dd if=$(KERNEL_BIN) of=$(FINAL_IMAGE) bs=512 seek=1 conv=notrunc
	@if [ -f $(FS_IMAGE) ]; then \
		echo "Injecting FAT16 partition from $(FS_IMAGE)..."; \
		dd if=$(FS_IMAGE) of=$(FINAL_IMAGE) bs=512 seek=2048 conv=notrunc; \
	fi
	@echo "--- Build Finished: $(FINAL_IMAGE) is ready ---"

# 2. Kompilace bootloaderu (Flat binary)
$(BOOT_BIN): fe-boot.asm
	@echo "Assembling bootloader: $<"
	$(NASM) -f bin $< -o $@

# 3. Ořezání ELF souboru na čistou binárku (flat binary pro kernel)
$(KERNEL_BIN): $(KERNEL_ELF)
	@echo "Stripping ELF to binary..."
	$(OBJCOPY) -O binary $(KERNEL_ELF) $(KERNEL_BIN)

# 4. Linkování kernelu do ELF
# Zde je kritické, aby tam byly ALL_OBJECTS
$(KERNEL_ELF): $(ALL_OBJECTS)
	@echo "Linking kernel..."
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(ALL_OBJECTS)

# 5. Pravidlo pro kompilaci .c souborů
%.o: %.c
	@echo "Compiling C: $<"
	$(CC) $(CFLAGS) -c $< -o $@

# 6. Pravidlo pro kompilaci .asm souborů (do objektového formátu ELF)
%.o: %.asm
	@echo "Assembling ASM: $<"
	$(NASM) -f elf32 $< -o $@

# --- Pomocné cíle ---

clean:
	@echo "Cleaning up..."
	rm -f $(BOOT_BIN) $(KERNEL_BIN) $(KERNEL_ELF) $(FINAL_IMAGE)
	find $(SRCDIR) -name '*.o' -delete

run: $(FINAL_IMAGE)
	qemu-system-i386 -drive format=raw,file=$(FINAL_IMAGE)

.PHONY: all clean run