#include <stdint.h>

#define MULTIBOOT_MAGIC 0x1BADB002
#define MULTIBOOT_FLAGS 3

struct multiboot_header {
    uint32_t magic;
    uint32_t flags;
    uint32_t checksum;
} __attribute__((packed, section(".multiboot")));

struct multiboot_header header = {
    .magic = MULTIBOOT_MAGIC,
    .flags = MULTIBOOT_FLAGS,
    .checksum = -(MULTIBOOT_MAGIC + MULTIBOOT_FLAGS)
};

void kmain(void) {
    uint16_t *vga = (uint16_t *)0xb8000;
    
    // Blauer Hintergrund
    for (int i = 0; i < 80 * 25; i++) {
        vga[i] = (0x01 << 8) | ' ';
    }
    
    // Titlebar
    for (int x = 0; x < 80; x++) {
        vga[x] = (0x70 << 8) | ' ';
    }
    
    // Text in titlebar
    const char *title = "Neutra OS 1.0";
    for (int x = 0; title[x]; x++) {
        vga[x + 2] = (0x70 << 8) | title[x];
    }
    
    // Fenster Box
    for (int x = 10; x < 40; x++) {
        vga[3 * 80 + x] = (0x0F << 8) | '*';
        vga[15 * 80 + x] = (0x0F << 8) | '*';
    }
    for (int y = 3; y < 15; y++) {
        vga[y * 80 + 10] = (0x0F << 8) | '*';
        vga[y * 80 + 40] = (0x0F << 8) | '*';
    }
    
    // Text in Fenster
    const char *text1 = "[ File ] [ Edit ] [ View ]";
    for (int x = 0; text1[x]; x++) {
        vga[5 * 80 + 12 + x] = (0x0F << 8) | text1[x];
    }
    
    const char *text2 = "Hello from Neutra OS!";
    for (int x = 0; text2[x]; x++) {
        vga[8 * 80 + 12 + x] = (0x0F << 8) | text2[x];
    }
    
    // Taskbar
    for (int x = 0; x < 80; x++) {
        vga[24 * 80 + x] = (0x07 << 8) | ' ';
    }
    
    const char *start = "Start";
    for (int x = 0; start[x]; x++) {
        vga[24 * 80 + 2 + x] = (0x70 << 8) | start[x];
    }
    
    while (1) {
        __asm__ volatile("hlt");
    }
}
