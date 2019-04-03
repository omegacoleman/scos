global flashcolor
global changecolor

[SECTION .text]

changecolor:
    push ebp
    mov ebp, esp
    pushad
    xor eax, eax
    mov eax, [ebp + 8]
    mov ebx, 2
    mul bx
    add eax, 0B8000h
    mov bx, [gs:eax]
    mov bh, [ebp + 12]
    mov [gs:eax],bx
    popad
    pop ebp
    ret

flashcolor:
    push ebp
    mov ebp, esp
    push eax
    push ebx
    mov ebx, [ebp + 8]
    add ebx, ebx
    add ebx, 0b8000h
    mov ax, [ebx]
    inc ah
    mov [ebx], ax
    pop ebx
    pop eax
    pop ebp
    ret

