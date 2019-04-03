
global startuppaging

[SECTION .text]

startuppaging:
    push ebp
    mov ebp, esp
    pushad
    mov esi, [ebp + 8]
    mov ecx, PageDirs
    mov edx, PageTables
    mov ebx, 0
    mov edi, 0
.loop:
    add edi, 1024
    inc ebx
    mov eax, 7
    or eax, edx
    mov [ecx], eax
    add ecx, 4
    add edx, 4096
    cmp ebx, esi
    jnz .loop

    mov ecx, PageTables
    mov edx, 0
    mov ebx, 0

.loop2:
    inc ebx
    mov eax, 7
    or eax, edx
    mov [ecx], eax
    add ecx, 4
    add edx, 4096
    cmp ebx, edi
    jnz .loop2

    cli
    mov eax, PageDirs
    mov cr3, eax
    mov eax, cr0
    or eax, 80000000h
    mov cr0, eax
    popad
    pop ebp
    ret


PageDirs equ 0x200000
PageTables equ 0x201000




