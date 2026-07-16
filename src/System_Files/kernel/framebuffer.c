#include <stdint.h>
#include <stddef.h>
#include "limine.h"

static uint32_t *framebuffer_addr;
static uint32_t framebuffer_width;
static uint32_t framebuffer_height;
static uint32_t framebuffer_pitch;

void framebuffer_init(void *boot_info) {
    struct limine_boot_info *boot = (struct limine_boot_info *)boot_info;
    
    if (boot == NULL || boot->framebuffer == NULL) {
        return;
    }
    
    framebuffer_addr = (uint32_t *)boot->framebuffer->address;
    framebuffer_width = boot->framebuffer->width;
    framebuffer_height = boot->framebuffer->height;
    framebuffer_pitch = boot->framebuffer->pitch;
}

void framebuffer_clear(void) {
    for (uint32_t i = 0; i < framebuffer_width * framebuffer_height; i++) {
        framebuffer_addr[i] = 0x000000;
    }
}

void framebuffer_putpixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x >= framebuffer_width || y >= framebuffer_height) {
        return;
    }
    
    uint32_t offset = (y * (framebuffer_pitch / 4)) + x;
    framebuffer_addr[offset] = color;
}