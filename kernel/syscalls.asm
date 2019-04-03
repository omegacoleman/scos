
global SYSprintstr
global SYSscanstr
global SYSkillme
global SYSnewproc
global SYSkillproc
global SYSdodiskreset
global SYSprintcolorstr
global SYSdevicereset
global SYSdoblock
global SYSgetvs
global SYSmalloc
global SYSfree

[SECTION .text]

SYSprintstr:
    push eax
    push ebx
    mov eax, 0x10
    mov ebx, [esp + 12]
    int 0x4f
    pop ebx
    pop eax
    ret

SYSprintcolorstr:
    push eax
    push ebx
    push ecx
    mov eax, 0x13
    mov ebx, [esp + 16]
    mov ecx, [esp + 20]
    int 0x4f
    pop ecx
    pop ebx
    pop eax
    ret

SYSscanstr:
    push eax
    push ebx
    mov eax, 0x11
    mov ebx, [esp + 12]
    int 0x4f
    pop ebx
    pop eax
.loop:
    mov eax, 0x12
    int 0x4f
    cmp eax, 0
    jnz .loop
    ret

SYSkillme:
    push eax
    mov eax, 0x1e
    int 0x4f
    pop eax
    ret

SYSnewproc:
    push ecx
    push ebx
    mov eax, 0x1d
    mov ebx, [esp + 12]
    mov ecx, [esp + 16]
    int 0x4f
    pop ebx
    pop ecx
    ret

SYSkillproc:
    push eax
    push ebx
    mov eax, 0x1a
    mov ebx, [esp + 12]
    int 0x4f
    pop ebx
    pop eax
    ret

SYSdevicereset:
    push eax
    push ebx
    mov eax, 0x52
    mov ebx, [esp + 12]
    int 0x4f
    pop ebx
    pop eax
    ret

SYSdoblock:
    push ebp
    mov ebp, esp
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi
    mov eax, 0x51
    mov ebx, [ebp + 8]
    mov ecx, [ebp + 12]
    mov edx, [ebp + 16]
    mov esi, [ebp + 20]
    mov edi, [ebp + 24]
    push ebp
    mov ebp, [ebp + 28]
    int 0x4f
    pop ebp
    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax
    pop ebp
    ret

SYSgetvs:
    mov eax, 0x80
    int 0x4f
    ret

SYSmalloc:
    push ebx
    mov ebx, [esp + 8]
    mov eax, 0x60
    int 0x4f
    pop ebx
    ret

SYSfree:
    push ebx
    push eax
    mov ebx, [esp + 12]
    mov eax, 0x61
    int 0x4f
    pop eax
    pop ebx
    ret



