#ifndef __LIMINE_H__
#define __LIMINE_H__

#include <stdint.h>

struct limine_framebuffer {
    uint64_t address;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
};

struct limine_boot_info {
    struct limine_framebuffer *framebuffer;
};

#endif
