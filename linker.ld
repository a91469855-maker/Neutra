ENTRY(_start)

SECTIONS {
    . = 0xFFFFFFFF80000000;
    
    .multiboot : AT(ADDR(.multiboot) - 0xFFFFFFFF80000000) {
        *(.multiboot)
    }
    
    .text : AT(ADDR(.text) - 0xFFFFFFFF80000000) {
        *(.text)
    }
    
    .data : AT(ADDR(.data) - 0xFFFFFFFF80000000) {
        *(.data)
    }
    
    .bss : AT(ADDR(.bss) - 0xFFFFFFFF80000000) {
        *(.bss)
    }
}
