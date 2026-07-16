CC = gcc
LD = ld

CFLAGS = -ffreestanding -fno-stack-protector -fno-builtin -Wall -Wextra -I. -static -no-pie

KERNEL = build/kernel.elf

OBJ = build/boot.o \
      build/kernel.o \
      build/scheduler.o \
      build/framebuffer.o \
      build/interrupthandler.o

all: $(KERNEL)

$(KERNEL): $(OBJ)
	mkdir -p build
	$(LD) -T linker.ld -static -no-pie -o $@ $^

build/boot.o: src/System_Files/kernel/boot.asm
	mkdir -p build
	nasm -f elf64 $< -o $@

build/%.o: src/System_Files/kernel/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build/

.PHONY: all clean