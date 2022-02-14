MBALIGN equ 1 << 0
MEMINFO equ 1 << 1
FRAMEBUFFER equ 1 << 12
FLAGS equ MBALIGN | MEMINFO | FRAMEBUFFER
MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

section .multiboot
align 4
        dd MAGIC
        dd FLAGS
        dd CHECKSUM

section .bss
align 16
        stack_bottom:
        resb 16384          ; 16 KB stack.
        stack_top:

section .text
global _start:function (_start.end - _start)
_start:
        mov esp, stack_top  ; small kernel stack

        push eax            ; Multiboot MAGIC number.
        push ebx            ; Multiboot Info Struct Address.

        extern kernel_main
        call kernel_main    ; call kernel C entry point

        cli
.hang:  hlt
        jmp .hang
.end:

