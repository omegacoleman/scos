
;
; This file is for the interrupts and the cpu params needed
; like gdt, idt. In fact, we have set these in loader, but
; very simple to work. This file is under nasm.
;


global changegdt
global changeidt
global setdoint
global zerointentry
global light
global timeirq

global syscallentry
global syscall
extern userlinertophys
global syscalluserpg


; For debug.
global undefined
global nint


global TSS_DESC
global LDT_DESC


extern interrupted
extern syscalled
extern timeint

extern entered
extern emptyclocks


[SECTION .text]

; For interrupts hasn't stack
times 1024 db 0
kernel_stack_buff:

;
; These are normal int entries.
; They push their interrupts into 
; stack, and jump into c method interrupted.
;
; Entries:
; int0 -- zerointentry
; int1 -- zerointentry + 10
; int2 -- zerointentry + 10 + 10
; int3 -- zerointentry + 10 + 10 + 10
; ...
;
zerointentry:
%assign i 0
%rep 256 
    push i
    jmp defint
%assign i i+1
%endrep

defint:
    cli
    mov eax, cr3
    mov [.zpg], eax
    mov [.irstack_addr], esp
    mov eax, 0x200000
    mov cr3, eax
    mov eax, esp
    mov esp, kernel_stack_buff
    push eax
    call userlinertophys
    mov esp, eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    call interrupted
    push eax
    mov eax, [.zpg]
    mov cr3, eax
    mov esp, [.irstack_addr]
    sub esp, 4
    mov ax, 0x17
    mov ds, ax
    mov es, ax
    pop eax
    add esp, 4
    sti
    iretd
    .zpg dd 0
    .irstack_addr dd 0


;
; Global Description Table -- GDT
;
GdtStart:
; We don't use first node
dw 0, 0, 0, 0
; Code segment.
dw 0FFFFh, 0h, 9A00h, 0CFh
; Data segment.
dw 0FFFFh, 0h, 9200h, 0C0h
; LDT segment
LDT_DESC:
dw 0, 0, 0, 0
; TSS segment
TSS_DESC:
times 252 dw 0, 0, 0, 0
GdtEnd:


;
; Interrupt Descriptor Table -- IDT
;
IdtStart:
%rep 256
dw 0h, 08h, 0EE00h, 0h
%endrep
IdtEnd:

;
; Pointers : structs for loading.
;

GdtPointer:
    dw 256 * 8
    dd GdtStart

IdtPointer:
    dw 256 * 8
    dd IdtStart

;
; Load the gdt pointer, then init.
;
; CS will be init when it returns.
;
changegdt:
    lgdt [GdtPointer]
    mov ax, 010h
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    ret ; This step init the cs

;
; Load the idt pointer.
;
changeidt:
    lidt [IdtPointer]
    ret

;
; Set the normal int process method.
;
setdoint:
    push ebp
    mov ebp, esp
    pushad
    mov edx, [ebp + 12]
    mov eax, edx
    mov bx, ax
    shr eax, 16
    mov ecx, IdtStart
    mov esi, [ebp + 8]
.loop:
    cmp esi, 0
    jz .ok
    add ecx, 8h
    sub esi, 1h
    jmp .loop
.ok:
    mov [ecx], bx
    add ecx, 6
    mov [ecx], ax
    popad
    pop ebp
    lidt [IdtPointer]
    ret

timeirq:
    cli
    pushad
    mov ax, 0x20
    out 0x20, al
    call timeint
    popad
    iretd


syscallentry:
    cli
    push edx
    push ecx
    push ebx
    push eax
    push ebp
    push edi
    push esi
    mov eax, cr3
    mov [.zpg], eax
    mov [.irstack_addr], esp
    mov eax, 0x200000
    mov cr3, eax
    mov eax, esp
    mov esp, kernel_stack_buff
    push eax
    call userlinertophys
    mov esp, eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    call syscalled
    push eax
    mov eax, [.zpg]
    mov cr3, eax
    mov esp, [.irstack_addr]
    sub esp, 4
    mov ax, 0x17
    mov ds, ax
    mov es, ax
    pop eax
    add esp, 4*7
    iretd
    .zpg dd 0
    .irstack_addr dd 0

light:
    mov eax, 0x78
    int 0x4f
    jmp light


