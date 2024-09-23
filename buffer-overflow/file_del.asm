section .text
global _start
_start:
    jmp short ender
starter:
    mov al, 8
    add al, 2
    mov ebx, _filename
    int 0x80
_exit:

    mov al, 1
    int 0x80

ender:
    call starter
_filename:
    db 'dummyfile', 0

