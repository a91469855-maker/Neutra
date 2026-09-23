extern keyboard_handler

global idt_load_asm
global keyboard_handler_asm

section .text
bits 32

idt_load_asm:
    mov eax, [esp + 4]
    lidt [eax]
    ret

keyboard_handler_asm:
    pusha
    call keyboard_handler
    popa
    iretd