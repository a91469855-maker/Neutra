#include "idt.h"
#include "vga.h"
#include "kernel.h"

uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

void outb(uint16_t port, uint8_t value) {
    __asm__ volatile("outb %0, %1" : : "a" (value), "Nd" (port));
}

static const uint8_t scancode_to_ascii[128] = {
    0,    0,  '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

void idt_init(void) {
    vga_write("IDT: Polling mode enabled\n", MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
}

void keyboard_handler_install(void) {
    vga_write("IDT: Keyboard ready (polling)\n", MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
}

char keyboard_getchar_poll(void) {
    while (1) {
        uint8_t status = inb(KEYBOARD_STATUS_PORT);
        if (status & 1) {
            uint8_t scancode = inb(KEYBOARD_DATA_PORT);
            if (scancode < 128) {
                char c = scancode_to_ascii[scancode];
                if (c != 0) {
                    return c;
                }
            }
        }
        __asm__ volatile("pause");
    }
}