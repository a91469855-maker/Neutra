#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_ENTRIES 256
#define PIC_MASTER_CMD 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CMD 0xA0
#define PIC_SLAVE_DATA 0xA1
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

typedef struct {
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t flags;
    uint16_t offset_high;
} __attribute__((packed)) IDT_Entry;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) IDT_Descriptor;

void idt_init(void);
void keyboard_handler_install(void);
char keyboard_getchar_poll(void);

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t value);

#endif