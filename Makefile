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

# --- Detekce zdrojových souborů kernelu ---
SOURCES = $(shell find $(SRCDIR) -name '*.c')
OBJECTS = $(SOURCES:.c=.o)

# --- Kompilační flagy ---
# -m32: 32-bit kód
# -ffreestanding: nepoužívat standardní knihovnu
# -fno-pic: zakázat pozičně nezávislý kód (důležité pro kernel)
CFLAGS = -m32 -ffreestanding -fno-pic -fno-stack-protector \
         -fno-asynchronous-unwind-tables -nostdlib -I$(INCDIR) -Wall -Wextra

# --- Linkovací flagy ---
# -T link.ld: cesta k tvému linker skriptu
LDFLAGS = -m elf_i386 -T link.ld

# --- Hlavní cíle ---

all: $(FINAL_IMAGE)

# 1. Tvorba finálního SD obrazu
$(FINAL_IMAGE): $(BOOT_BIN) $(KERNEL_BIN)
	@echo "--- Creating 32MB raw disk image with MBR ---"
	# Vytvoření prázdného souboru (32 MB)
	dd if=/dev/zero of=$(FINAL_IMAGE) bs=1M count=32
	
	# Zápis MBR Partition Table pomocí sfdisk
	# Vytvoří partition od sektoru 2048, typ 6 (FAT16), označí jako bootable (*)
	@echo "2048,,6,*" | sfdisk $(FINAL_IMAGE) > /dev/null 2>&1

	# Zápis kódu bootloaderu do MBR (pouze prvních 446 bajtů, abychom nepřepsali PT)
	dd if=$(BOOT_BIN) of=$(FINAL_IMAGE) bs=446 count=1 conv=notrunc
	
	# Zápis signatury 0xAA55 na konec prvního sektoru (offset 510)
	dd if=$(BOOT_BIN) of=$(FINAL_IMAGE) bs=1 skip=510 seek=510 count=2 conv=notrunc

	# Zápis kernelu od sektoru 1 (LBA 1)
	dd if=$(KERNEL_BIN) of=$(FINAL_IMAGE) bs=512 seek=1 conv=notrunc

	# Pokud existuje fat16.img, vložíme ho na sektor 2048
	@if [ -f $(FS_IMAGE) ]; then \
		echo "Injecting FAT16 partition from $(FS_IMAGE)..."; \
		dd if=$(FS_IMAGE) of=$(FINAL_IMAGE) bs=512 seek=2048 conv=notrunc; \
	else \
		echo "Warning: $(FS_IMAGE) not found, partition area at LBA 2048 will be empty."; \
	fi
	@echo "--- Build Finished: $(FINAL_IMAGE) is ready ---"

# 2. Kompilace bootloaderu
$(BOOT_BIN): fe-boot.asm
	@echo "Assembling bootloader..."
	$(NASM) -f bin fe-boot.asm -o $(BOOT_BIN)

# 3. Ořezání ELF souboru na čistou binárku (flat binary)
$(KERNEL_BIN): $(KERNEL_ELF)
	@echo "Stripping ELF to binary..."
	$(OBJCOPY) -O binary $(KERNEL_ELF) $(KERNEL_BIN)

# 4. Linkování kernelu do ELF
$(KERNEL_ELF): $(OBJECTS) $(GDT)
	@echo "Linking kernel..."
	$(LD) $(LDFLAGS) -o $(KERNEL_ELF) $(OBJECTS)

# 5. Generické pravidlo pro kompilaci .c souborů
%.o: %.c
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.asm
	@echo "Compiling $<..."
	nasm -f elf32 $< -o $@

# --- Pomocné cíle ---

# Vyčištění projektu
clean:
	@echo "Cleaning up..."
	rm -f $(BOOT_BIN) $(KERNEL_BIN) $(KERNEL_ELF)
	find $(SRCDIR) -name '*.o' -delete

# Spuštění v QEMU (volitelné, uprav podle potřeby)
run: $(FINAL_IMAGE)
	qemu-system-i386 -drive format=raw,file=$(FINAL_IMAGE)

.PHONY: all clean run