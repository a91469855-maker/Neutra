#include <stdint.h>
#include <stddef.h>
#include "kernel.h"
#include "scheduler.h"
#include "vga.h"
#include "idt.h"
#include "shell.h"
#include "dce.h"
#include "graphics.h"
#include "kernel_memory/program.h"

uint32_t mbt_addr = 0;

void int_to_str(int num, char* buf, int buf_size) {
    if (buf_size < 2) return;
    if (num == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    int is_negative = (num < 0);
    if (is_negative) num = -num;
    int i = 0;
    int temp = num;
    while (temp > 0 && i < buf_size - 2) {
        buf[i++] = '0' + (temp % 10);
        temp /= 10;
    }
    if (is_negative && i < buf_size - 2) {
        buf[i++] = '-';
    }
    buf[i] = '\0';
    for (int j = 0; j < i / 2; j++) {
        char tmp = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = tmp;
    }
}

void hex_to_str(unsigned int num, char* buf, int buf_size) {
    if (buf_size < 3) return;
    const char* hex_chars = "0123456789abcdef";
    int i = 0;
    if (num == 0) {
        buf[0] = '0';
        buf[1] = '\0';
        return;
    }
    while (num > 0 && i < buf_size - 1) {
        buf[i++] = hex_chars[num & 0xF];
        num >>= 4;
    }
    buf[i] = '\0';
    for (int j = 0; j < i / 2; j++) {
        char tmp = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = tmp;
    }
}

void memset(void* ptr, int value, size_t num) {
    unsigned char* p = (unsigned char*)ptr;
    for (size_t i = 0; i < num; i++) {
        p[i] = (unsigned char)value;
    }
}

void memcpy(void* dest, const void* src, size_t num) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    for (size_t i = 0; i < num; i++) {
        d[i] = s[i];
    }
}

int strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return (unsigned char)*str1 - (unsigned char)*str2;
}

void shell_task(void) {
    idt_init();
    keyboard_handler_install();
    shell_run();
}

void run_program(void) {

    DCE_VM vm;
    dce_vm_init(&vm);

    dce_vm_load_cde(&vm, program_cde, program_cde_len);
    const CDE_Header* h = (const CDE_Header*)program_cde;


    dce_vm_execute(&vm);


}

static int cde_started = 0;

void execute_cde_task(void) {
    if (cde_started)
        return;

    cde_started = 1;
    run_program();
}

void test_framebuffer(void) {
    if (!fb_info.has_framebuffer) {
        vga_write("no framebuffer available\n", MAKE_COLOR(COLOR_BLACK, COLOR_RED));
        return;
    }
    
    int center_x = (fb_info.framebuffer_width / 2) - 4;
    int center_y = (fb_info.framebuffer_height / 2) - 4;
    
    uint32_t red = 0xFF0000;
    draw_rect(center_x, center_y, 8, 8, red);
    
    vga_write("done\n", MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
}

void kernel_entry(uint32_t mbt_addr) {
    vga_clear();
    vga_write("Neutra Kernel\n", MAKE_COLOR(COLOR_BLACK, COLOR_GREEN));
    
    __asm__ volatile ("movl %%ebx, %0" : "=r" (mbt_addr));
        parse_multiboot_tags(mbt_addr);
    
    scheduler_init();
    scheduler_add_task("Shell", shell_task, 10000000);
    scheduler_run();
    
    vga_write("Kernel halted\n", MAKE_COLOR(COLOR_BLACK, COLOR_CYAN));
    
    while (1) {
        __asm__ volatile("hlt");
    }
}