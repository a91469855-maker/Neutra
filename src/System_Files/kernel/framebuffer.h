#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>

void framebuffer_init(void *boot_info);
void framebuffer_clear(void);
void framebuffer_putpixel(uint32_t x, uint32_t y, uint32_t color);

#endif