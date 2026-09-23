#include "graphics.h"
#include "vga.h"

FramebufferInfo fb_info = {0};

void parse_multiboot_tags(uint32_t mbt_addr) {
    if (mbt_addr == 0) return;
    
    vga_write("Parse tags at 0x", MAKE_COLOR(COLOR_BLACK, COLOR_CYAN));
    vga_print_hex(mbt_addr, MAKE_COLOR(COLOR_BLACK, COLOR_CYAN));
    vga_write("\n", MAKE_COLOR(COLOR_BLACK, COLOR_CYAN));
    
    const MultibootTag* tag = (const MultibootTag*)(mbt_addr + 8);
    int tag_count = 0;
    
    while (tag->type != MULTIBOOT_TAG_TYPE_END && tag_count < 10) {
        vga_write("Tag type=", MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_YELLOW));
        vga_print_int(tag->type, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_YELLOW));
        vga_write(" size=", MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_YELLOW));
        vga_print_int(tag->size, MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_YELLOW));
        vga_write("\n", MAKE_COLOR(COLOR_BLACK, COLOR_LIGHT_YELLOW));
        
        if (tag->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {
            const MultibootTagFramebuffer* fb = 
                (const MultibootTagFramebuffer*)tag;
            
            fb_info.framebuffer_addr = fb->framebuffer_addr;
            fb_info.framebuffer_width = fb->framebuffer_width;
            fb_info.framebuffer_height = fb->framebuffer_height;
            fb_info.framebuffer_pitch = fb->framebuffer_pitch;
            fb_info.framebuffer_bpp = fb->framebuffer_bpp;
            fb_info.has_framebuffer = 1;
            
            vga_write("FB FOUND!\n", MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
        }
        
        tag = (const MultibootTag*)
            ((uint8_t*)tag + ((tag->size + 7) & ~7));
        tag_count++;
    }
}

void putpixel(int x, int y, uint32_t color) {
    if (!fb_info.has_framebuffer) return;
    if (x < 0 || x >= (int)fb_info.framebuffer_width) return;
    if (y < 0 || y >= (int)fb_info.framebuffer_height) return;
    
    uint32_t offset = y * fb_info.framebuffer_pitch + 
                      x * (fb_info.framebuffer_bpp / 8);
    
    volatile uint32_t* pixels = 
        (volatile uint32_t*)(fb_info.framebuffer_addr + offset);
    
    if (fb_info.framebuffer_bpp == 32) {
        *pixels = color;
    }
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {
    for (int yy = y; yy < y + height; yy++) {
        for (int xx = x; xx < x + width; xx++) {
            putpixel(xx, yy, color);
        }
    }
}

void fb_clear(uint32_t color) {
    if (!fb_info.has_framebuffer) return;
    draw_rect(0, 0, fb_info.framebuffer_width, 
              fb_info.framebuffer_height, color);
}
