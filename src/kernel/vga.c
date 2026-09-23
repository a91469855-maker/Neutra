#include <stdint.h>
#include <stddef.h>

#include "kernel.h"
#include "vga.h"

static volatile int vga_x = 0;
static volatile int vga_y = 0;

#define MAKE_COLOR(bg, fg) (((bg) << 4) | (fg))

void vga_clear(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_MEMORY[i] = 0x0000;
    }
    vga_x = 0;
    vga_y = 0;
}

void vga_scroll(void) {
    
    for (int i = 0; i < VGA_WIDTH * (VGA_HEIGHT - 1); i++) {
        VGA_MEMORY[i] = VGA_MEMORY[i + VGA_WIDTH];
    }
    
    for (int i = VGA_WIDTH * (VGA_HEIGHT - 1); i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_MEMORY[i] = 0x0000;
    }
    vga_y = VGA_HEIGHT - 1;
    vga_x = 0;
}

void vga_putchar(char c, unsigned char color) {
    if (c == '\n') {
        vga_x = 0;
        vga_y++;
    } else {
        int index = vga_y * VGA_WIDTH + vga_x;
        
        if (index < VGA_WIDTH * VGA_HEIGHT) {
            VGA_MEMORY[index] = ((uint16_t)color << 8) | (unsigned char)c;
            vga_x++;
        }
    }

    if (vga_x >= VGA_WIDTH) {
        vga_x = 0;
        vga_y++;
    }

    if (vga_y >= VGA_HEIGHT) {
        vga_scroll();
    }
}

void vga_write(const char* str, unsigned char color) {
    if (str == NULL) {
        return;
    }

    while (*str) {
        vga_putchar(*str++, color);
    }
}

void vga_print_hex(unsigned int num, unsigned char color) {
    const char* hex_chars = "0123456789abcdef";
    char buf[16];
    int i = 0;

    if (num == 0) {
        vga_putchar('0', color);
        return;
    }

    unsigned int temp = num;

    while (temp > 0) {
        buf[i++] = hex_chars[temp & 0xF];
        temp >>= 4;
    }

    for (int j = i - 1; j >= 0; j--) {
        vga_putchar(buf[j], color);
    }
}

void vga_print_int(int num, unsigned char color) {
    char buf[32];
    int_to_str(num, buf, sizeof(buf));
    vga_write(buf, color);
}