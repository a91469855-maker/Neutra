global _start

extern kmain
extern nscb_init

section .text

_start:
    mov rsp, stack + 4096

    call nscb_init

    cmp eax, 1
    jne .security_failed

    call kmain

.hang:
    cli
    hlt
    jmp .hang


.security_failed:
    cli
    hlt
    jmp .security_failed


section .bss
stack:
    resb 4096