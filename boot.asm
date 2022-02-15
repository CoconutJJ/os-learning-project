MBALIGN equ 1 << 0
MEMINFO equ 1 << 1
VIDEO equ 1 << 2

FLAGS equ MBALIGN | MEMINFO | VIDEO

MAGIC    equ  0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

; not used, garbage padding values.
HEADER_ADDR equ 0
LOAD_ADDR equ 0
LOAD_END_ADDR equ 0
BSS_END_ADDR equ 0
ENTRY_ADDR equ 0

; RGB: linear graphics mode?
MODE_TYPE equ 0
; no preference.
WIDTH equ 0
HEIGHT equ 0
DEPTH equ 0

extern kernel_main

section .multiboot
align 4
        dd MAGIC
        dd FLAGS
        dd CHECKSUM
        dd HEADER_ADDR
        dd LOAD_ADDR
        dd LOAD_END_ADDR
        dd BSS_END_ADDR
        dd ENTRY_ADDR
        dd MODE_TYPE
        dd WIDTH
        dd HEIGHT
        dd DEPTH
section .bss
align 16
        stack_bottom:
        resb 16384          ; 16 KB stack.
        stack_top:

section .text
global _start:function (_start.end - _start)
_start:
        mov esp, stack_top  ; small kernel stack
        
        push ebx            ; Multiboot Info Struct Address.
        push eax            ; Multiboot MAGIC number.

        call kernel_main    ; call kernel C entry point

        cli
.hang:  hlt
        jmp .hang
.end:

