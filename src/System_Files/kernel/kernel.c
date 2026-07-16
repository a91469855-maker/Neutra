#include <stdint.h>

__attribute__((section(".multiboot")))
int multiboot_header[] = {
    0xe85250d6,
    0,
    (8 + 0),
    -(0xe85250d6 + 0 + (8 + 0)),
    0, 0, 8
};

void serial_putchar(char c) {
    while ((*(volatile uint8_t *)0x3f8 & 0x20) == 0);
    *(volatile uint8_t *)0x3f8 = c;
}

void serial_write(const char *s) {
    while (*s) {
        serial_putchar(*s++);
    }
}

void kmain(void) {
    serial_write("KERNEL STARTED!\n");
    
    while (1) {
        __asm__ volatile("hlt");
    }
}