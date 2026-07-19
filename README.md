# NEUTRA OS - Complete Development Roadmap

Status: Phase 1 - Text Mode Foundation (Active Development)
Last Updated: July 2026
Bootloader: GRUB (Multiboot)
Target: Windows 95-Style Desktop OS

---

# TABLE OF CONTENTS

1. Overview
2. Phase 1: Text Mode Foundation
3. Phase 2: Extended Text Mode
4. Phase 3: Graphics Mode
5. Phase 4: Advanced Features
6. Build & Run

---

# OVERVIEW

Neutra OS is a from-scratch desktop operating system built in C and x86-64 Assembly. The development follows a phased approach:

Phase 1 (Current): Basic Text Mode Output, GRUB Boot, Multiboot Info
Phase 2: Window System, Text GUI, Menu Bar, Taskbar
Phase 3: Graphics Mode (1024x768), Pixel Drawing, Advanced GUI
Phase 4: Memory Management, Process Control, File System, Networking

Each phase is fully functional and bootable before moving to the next.

---

# PHASE 1: TEXT MODE FOUNDATION

Duration: 1-2 Weeks
Status: IN PROGRESS
Key Achievement: Bootable Kernel with Text Output

## 1.1 Bootloader Integration

Requirements:
- GRUB Multiboot Compliance
- Multiboot Header in boot.asm
- Entry Point at _start
- Stack Setup
- Pass Multiboot Info to kernel

Implementation

boot.asm:
```asm
extern kmain
global _start

section .multiboot
align 4
    dd 0x1BADB002
    dd 0x00000003
    dd -(0x1BADB002 + 0x00000003)

section .text
_start:
    mov %ebx, %edi
    lea rsp, [rel stack_top]
    xor rbp, rbp
    
    call kmain
    
    cli
    hlt
    jmp _start

section .bss
    align 4096
    stack:
        resb 32768
    stack_top:
```

Linker Script (linker.ld):
```
ENTRY(_start)

SECTIONS {
    . = 0x100000;
    
    .text : {
        *(.text)
        *(.text.*)
    }
    
    .rodata : {
        *(.rodata)
        *(.rodata.*)
    }
    
    .data : {
        *(.data)
        *(.data.*)
    }
    
    .bss : {
        *(.bss)
        *(COMMON)
    }
}
```

GRUB Configuration (iso/boot/grub/grub.cfg):
```
set default=0
set timeout=5

menuentry "Neutra OS" {
    multiboot /boot/kernel.elf
}
```

---

## 1.2 Multiboot Info Parsing

Data Structure:
```c
struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
    uint64_t fb_addr;
    uint32_t fb_pitch;
    uint32_t fb_width;
    uint32_t fb_height;
    uint8_t fb_bpp;
    uint8_t fb_type;
} __attribute__((packed));
```

Usage in kernel.c:
```c
void kmain(struct multiboot_info *mbi) {
    if (mbi == NULL) {
        while(1) __asm__ volatile("hlt");
    }
    
    uint32_t mem_upper = mbi->mem_upper;
    uint32_t bootloader = mbi->boot_loader_name;
}
```

---

## 1.3 Text Mode Output (VGA 0xB8000)

Text Screen Manager:
```c
#define TEXT_BUFFER    0xb8000
#define TEXT_WIDTH     80
#define TEXT_HEIGHT    25

typedef struct {
    uint16_t *buffer;
    uint32_t width;
    uint32_t height;
    uint32_t cursor_x;
    uint32_t cursor_y;
} text_screen_t;

text_screen_t screen;

#define COLOR_BLACK    0x00
#define COLOR_BLUE     0x01
#define COLOR_GREEN    0x02
#define COLOR_CYAN     0x03
#define COLOR_RED      0x04
#define COLOR_MAGENTA  0x05
#define COLOR_YELLOW   0x06
#define COLOR_WHITE    0x07

void text_init(void) {
    screen.buffer = (uint16_t *)TEXT_BUFFER;
    screen.width = TEXT_WIDTH;
    screen.height = TEXT_HEIGHT;
    screen.cursor_x = 0;
    screen.cursor_y = 0;
}

void text_clear(uint8_t bg, uint8_t fg) {
    uint8_t color = (bg << 4) | fg;
    for (int i = 0; i < TEXT_WIDTH * TEXT_HEIGHT; i++) {
        screen.buffer[i] = (color << 8) | ' ';
    }
    screen.cursor_x = 0;
    screen.cursor_y = 0;
}

void text_putchar(char c, uint8_t color) {
    if (c == '\n') {
        screen.cursor_y++;
        screen.cursor_x = 0;
        return;
    }
    
    screen.buffer[screen.cursor_y * TEXT_WIDTH + screen.cursor_x] = 
        (color << 8) | c;
    
    screen.cursor_x++;
    if (screen.cursor_x >= TEXT_WIDTH) {
        screen.cursor_x = 0;
        screen.cursor_y++;
    }
    
    if (screen.cursor_y >= TEXT_HEIGHT) {
        screen.cursor_y = TEXT_HEIGHT - 1;
    }
}

void text_puts(const char *str, uint8_t color) {
    while (*str) {
        text_putchar(*str++, color);
    }
}

void text_putint(int num, uint8_t color) {
    char buffer[16];
    int idx = 0;
    
    if (num == 0) {
        text_putchar('0', color);
        return;
    }
    
    while (num > 0) {
        buffer[idx++] = '0' + (num % 10);
        num /= 10;
    }
    
    while (idx > 0) {
        text_putchar(buffer[--idx], color);
    }
}
```

Kernel Entry:
```c
void kmain(struct multiboot_info *mbi) {
    text_init();
    text_clear(COLOR_BLUE, COLOR_WHITE);
    
    text_puts("=== NEUTRA OS 1.0 ===\n", 0x0F);
    text_puts("Bootloader: ", 0x0F);
    text_puts((const char *)mbi->boot_loader_name, 0x0F);
    text_puts("\n", 0x0F);
    
    text_puts("Memory: ", 0x0F);
    text_putint(mbi->mem_upper / 1024, 0x0F);
    text_puts(" KB\n", 0x0F);
    
    while (1) {
        __asm__ volatile("hlt");
    }
}
```

---

## 1.4 Build System

Makefile:
```makefile
CC = gcc
LD = ld
NASM = nasm
GRUB = grub-mkrescue
XORRISO = xorriso

CFLAGS = -ffreestanding -fno-stack-protector -fno-builtin \
         -Wall -Wextra -I. -static -no-pie
LDFLAGS = -T linker.ld -static -no-pie

KERNEL = build/kernel.elf

OBJ = build/boot.o \
      build/kernel.o

all: $(KERNEL)

$(KERNEL): $(OBJ)
	mkdir -p build
	$(LD) $(LDFLAGS) -o $@ $^

build/boot.o: src/System_Files/kernel/boot.asm
	mkdir -p build
	$(NASM) -f elf64 $< -o $@

build/%.o: src/System_Files/kernel/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

iso: $(KERNEL)
	rm -rf iso/
	mkdir -p iso/boot/grub
	cp $(KERNEL) iso/boot/
	cat > iso/boot/grub/grub.cfg << 'EOF'
set default=0
set timeout=5
menuentry "Neutra OS" {
    multiboot /boot/kernel.elf
}
EOF
	rm -f neutra.iso
	$(GRUB) -o neutra.iso iso/

run: iso
	qemu-system-x86_64 -cdrom neutra.iso -m 256M -vga std

debug: iso
	qemu-system-x86_64 -cdrom neutra.iso -m 256M -vga std -serial stdio

clean:
	rm -rf build/ iso/ neutra.iso

.PHONY: all iso run debug clean
```

---

# PHASE 2: EXTENDED TEXT MODE

Duration: 2-4 Weeks
Status: PLANNED
Goal: Windows 95-Style Text GUI

## 2.1 Window Manager

Window Structure:
```c
typedef struct {
    uint32_t x, y;
    uint32_t width, height;
    const char *title;
    uint8_t title_color;
    uint8_t bg_color;
    char *content;
} window_t;

window_t *windows[10];
uint32_t window_count = 0;

void window_create(uint32_t x, uint32_t y, uint32_t w, uint32_t h, 
                   const char *title) {
    if (window_count >= 10) return;
    
    window_t *win = &windows[window_count++];
    win->x = x;
    win->y = y;
    win->width = w;
    win->height = h;
    win->title = title;
    win->title_color = 0x70;
    win->bg_color = 0x0F;
}

void window_draw_border(window_t *win) {
    for (uint32_t i = 0; i < win->width; i++) {
        text_putchar_at(win->x + i, win->y, '*', 0x0F);
        text_putchar_at(win->x + i, win->y + win->height - 1, '*', 0x0F);
    }
    
    for (uint32_t i = 0; i < win->height; i++) {
        text_putchar_at(win->x, win->y + i, '*', 0x0F);
        text_putchar_at(win->x + win->width - 1, win->y + i, '*', 0x0F);
    }
}

void window_draw_titlebar(window_t *win) {
    for (uint32_t i = 0; i < win->width; i++) {
        text_putchar_at(win->x + i, win->y + 1, ' ', win->title_color);
    }
    
    uint32_t title_x = win->x + (win->width - strlen(win->title)) / 2;
    for (const char *c = win->title; *c; c++) {
        text_putchar_at(title_x++, win->y + 1, *c, win->title_color);
    }
}

void window_draw(window_t *win) {
    window_draw_border(win);
    window_draw_titlebar(win);
}

void windows_render_all(void) {
    for (uint32_t i = 0; i < window_count; i++) {
        window_draw(windows[i]);
    }
}
```

## 2.2 Menu Bar & Taskbar

Menu Bar:
```c
#define MENU_ITEMS 3

typedef struct {
    const char *label;
    const char **options;
    uint32_t option_count;
} menu_t;

menu_t menus[MENU_ITEMS] = {
    {"File", (const char*[]){"New", "Open", "Save", "Exit"}, 4},
    {"Edit", (const char*[]){"Cut", "Copy", "Paste"}, 3},
    {"View", (const char*[]){"Zoom In", "Zoom Out", "Full Screen"}, 3}
};

void menubar_draw(void) {
    uint32_t x = 0;
    for (uint32_t i = 0; i < MENU_ITEMS; i++) {
        text_puts_at(x, 1, "[ ", 0x70);
        text_puts_at(x + 2, 1, menus[i].label, 0x70);
        text_puts_at(x + 2 + strlen(menus[i].label), 1, " ]", 0x70);
        x += strlen(menus[i].label) + 4;
    }
}
```

Taskbar:
```c
void taskbar_draw(void) {
    for (uint32_t i = 0; i < TEXT_WIDTH; i++) {
        text_putchar_at(i, TEXT_HEIGHT - 1, ' ', 0x07);
    }
    
    text_puts_at(2, TEXT_HEIGHT - 1, "Start", 0x70);
    text_puts_at(TEXT_WIDTH - 10, TEXT_HEIGHT - 1, "00:00", 0x07);
}
```

## 2.3 Widget System

Button Widget:
```c
typedef struct {
    uint32_t x, y;
    uint32_t width, height;
    const char *label;
    void (*onclick)(void);
} button_t;

void button_draw(button_t *btn) {
    for (uint32_t i = 0; i < btn->width; i++) {
        text_putchar_at(btn->x + i, btn->y, ' ', 0x70);
        text_putchar_at(btn->x + i, btn->y + btn->height - 1, ' ', 0x70);
    }
    
    uint32_t label_x = btn->x + (btn->width - strlen(btn->label)) / 2;
    text_puts_at(label_x, btn->y, btn->label, 0x70);
}
```

---

# PHASE 3: GRAPHICS MODE

Duration: 4-8 Weeks
Status: PLANNED
Goal: 1024x768 32-bit Graphics, Pixel Drawing

## 3.1 GRUB Graphics Mode Setup

Updated GRUB Config:
```
set default=0
set timeout=5

insmod vbe
insmod vga
set gfxmode=1024x768x32
insmod gfxterm
terminal_output gfxterm

menuentry "Neutra OS" {
    multiboot /boot/kernel.elf
}
```

## 3.2 Framebuffer Management

Framebuffer Structure:
```c
typedef struct {
    uint32_t *pixels;
    uint32_t width;
    uint32_t height;
    uint32_t pitch;
    uint8_t bpp;
    uint8_t type;
} framebuffer_t;

framebuffer_t fb;

void fb_init(struct multiboot_info *mbi) {
    if (!(mbi->flags & (1 << 12))) {
        return;
    }
    
    fb.pixels = (uint32_t *)mbi->fb_addr;
    fb.width = mbi->fb_width;
    fb.height = mbi->fb_height;
    fb.pitch = mbi->fb_pitch;
    fb.bpp = mbi->fb_bpp;
    fb.type = mbi->fb_type;
}

void fb_putpixel(uint32_t x, uint32_t y, uint32_t color) {
    if (x >= fb.width || y >= fb.height) return;
    
    uint32_t offset = y * (fb.pitch / 4) + x;
    fb.pixels[offset] = color;
}

void fb_rect(uint32_t x, uint32_t y, uint32_t w, uint32_t h, uint32_t color) {
    for (uint32_t yy = y; yy < y + h; yy++) {
        for (uint32_t xx = x; xx < x + w; xx++) {
            fb_putpixel(xx, yy, color);
        }
    }
}

void fb_clear(uint32_t color) {
    fb_rect(0, 0, fb.width, fb.height, color);
}
```

## 3.3 Graphics GUI Elements

Window (Graphics):
```c
typedef struct {
    uint32_t x, y, width, height;
    uint32_t bg_color;
    uint32_t border_color;
    uint32_t title_color;
    const char *title;
} gfx_window_t;

void gfx_window_draw(gfx_window_t *win) {
    fb_rect(win->x, win->y, win->width, 20, win->title_color);
    
    fb_rect(win->x, win->y + 20, win->width, win->height - 20, 
            win->bg_color);
    
    fb_rect(win->x, win->y, 1, win->height, win->border_color);
    fb_rect(win->x + win->width - 1, win->y, 1, win->height, 
            win->border_color);
}
```

## 3.4 Font Rendering (Simple Bitmap Font)

```c
const uint8_t font_data[256][8] = {
};

void gfx_putchar(uint32_t x, uint32_t y, char c, uint32_t color) {
    const uint8_t *glyph = font_data[(unsigned char)c];
    
    for (int yy = 0; yy < 8; yy++) {
        uint8_t row = glyph[yy];
        for (int xx = 0; xx < 8; xx++) {
            if (row & (1 << (7 - xx))) {
                fb_putpixel(x + xx, y + yy, color);
            }
        }
    }
}

void gfx_puts(uint32_t x, uint32_t y, const char *str, uint32_t color) {
    while (*str) {
        gfx_putchar(x, y, *str++, color);
        x += 8;
    }
}
```

---

# PHASE 4: ADVANCED FEATURES

Duration: 8+ Weeks
Status: FUTURE

## 4.1 Memory Management

Physical Memory Manager:
- Detect Available RAM from Multiboot
- Page Frame Allocator (4KB pages)
- Bitmap-based Free/Used tracking

Virtual Memory:
- Page Tables Setup
- Virtual Address Space
- Higher-half kernel relocation

## 4.2 Interrupt Handling

IDT Setup:
- 256 interrupt descriptors
- Interrupt handlers
- Exception handling

Common Interrupts:
- Timer (IRQ0)
- Keyboard (IRQ1)
- System calls (INT 0x80)

## 4.3 Process Management

Task Structure:
```c
typedef struct {
    uint32_t pid;
    char name[32];
    uint32_t state;
    uint32_t priority;
} task_t;
```

Scheduler:
- Round-robin scheduling
- Context switching
- Task queue management

## 4.4 File System

FAT32 Support:
- Partition detection
- Directory navigation
- File read/write operations

## 4.5 Keyboard Input

PS/2 Keyboard Driver:
- Interrupt-driven input
- Scancode translation
- Input buffer management

## 4.6 Networking (Long-term)

Network Stack:
- Ethernet driver
- TCP/IP implementation
- Socket API

---

# BUILD & RUN

Requirements:
```bash
sudo apt-get install gcc g++ nasm grub-pc xorriso qemu-system-x86 make
```

Build Kernel:
```bash
make clean
make
```

Create Bootable ISO:
```bash
make iso
```

Run in QEMU:
```bash
make run
```

Debug Mode (with Serial Output):
```bash
make debug
```

---

# DIRECTORY STRUCTURE

```
neutra-os/
├── src/
│   └── System_Files/kernel/
│       ├── boot.asm
│       ├── kernel.c
│       ├── display.c
│       ├── memory.c
│       ├── interrupts.c
│       ├── scheduler.c
│       └── drivers/
│           ├── keyboard.c
│           ├── disk.c
│           └── network.c
├── build/
├── iso/
├── linker.ld
├── Makefile
├── README.md
└── LICENSE
```

---

# PROGRESS TRACKING

Phase 1 Status:
- [x] Boot from GRUB
- [x] Multiboot Info parsing
- [x] Text mode output (VGA 0xB8000)
- [x] Build system (Makefile)
- [ ] Scrolling support
- [ ] Extended text attributes

Phase 2 Status:
- [ ] Window system
- [ ] Menu bar
- [ ] Taskbar
- [ ] Button widgets
- [ ] Text input
- [ ] Dialog boxes

Phase 3 Status:
- [ ] Graphics mode setup
- [ ] Framebuffer initialization
- [ ] Pixel drawing
- [ ] Basic shapes (rect, circle)
- [ ] Bitmap font rendering
- [ ] Window manager (graphics)

Phase 4 Status:
- [ ] Physical memory manager
- [ ] Virtual memory / Paging
- [ ] IDT / Interrupt handling
- [ ] Process/Task management
- [ ] Scheduler
- [ ] File system (FAT32)
- [ ] Keyboard driver
- [ ] Network stack

---

# NEXT STEPS

Immediate (This Week):
- Complete Phase 1 text mode
- Add scrolling support
- Test with various GRUB configurations

Short-term (Next 2 Weeks):
- Begin Phase 2 window manager
- Implement basic menu system
- Add taskbar

Medium-term (1 Month):
- Complete Phase 2 GUI
- Start Phase 3 graphics mode
- Implement framebuffer code

Long-term (3+ Months):
- Complete graphics implementation
- Begin memory management (Phase 4)
- Add keyboard driver

---

# CONTRIBUTING

This is a personal learning project, but documentation, bugfixes, and architectural feedback are welcome.

---

# LICENSE

GNU General Public License v3.0 (GPL-3.0)
See LICENSE file for details.

---

Author: NeutriexD
Project Start: July 2026
Last Updated: July 2026

Current Status: Phase 1 - Active Development
Bootability: GRUB Multiboot Compliant
Display: Text Mode (VGA 0xB8000)
