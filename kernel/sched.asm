
global restart
global start


[SECTION .text]
restart:
    xor eax, eax
    mov ax, [esp + 4]
    mov ebx, .jmptss
    add ebx, 3 * 2 - 1
    mov [ebx], ax
.jmptss:
    jmp 0x1234:0
    ret

start:
    jmp 0x20:0
    iretd

