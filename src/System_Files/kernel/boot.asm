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