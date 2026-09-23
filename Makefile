AS = nasm
CC = i686-linux-gnu-gcc
LD = i686-linux-gnu-ld

CFLAGS = -m32 -nostdlib -nostartfiles -nodefaultlibs -ffreestanding -fno-pie -no-pie -Wall -Wextra
LDFLAGS = -m elf_i386 -T src/linker.ld -nostdlib

BOOT_OBJS = src/boot/header.o src/boot/main.o
KERNEL_OBJS = src/kernel/kernel.o src/kernel/vga.o src/kernel/scheduler.o src/kernel/dce.o src/kernel/idt.o src/kernel/shell.o src/kernel/graphics.o

ALL_OBJS = $(BOOT_OBJS) $(KERNEL_OBJS)

OUTPUT = kernel.elf
ISO_OUTPUT = os.iso

.PHONY: all clean iso run-iso

all: $(OUTPUT)

$(OUTPUT): $(ALL_OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

%.o: %.asm
	$(AS) -f elf32 $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

iso: $(OUTPUT)
	mkdir -p iso/boot/grub
	cp $(OUTPUT) iso/boot/kernel.elf
	cp src/boot/grub/grub.cfg iso/boot/grub/
	grub-mkrescue -o $(ISO_OUTPUT) iso 2>/dev/null
	rm -rf iso

run-iso: iso
	qemu-system-i386  $(ISO_OUTPUT) -m 256 -vga std

clean:
	rm -f $(ALL_OBJS) $(OUTPUT) $(ISO_OUTPUT)
	rm -rf iso
