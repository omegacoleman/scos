;
; Then, bootsector copied us a new address, 0x7e00
; We are following the bootsector.
;
    org 07e00h
;
; The main body of the code.
;
    ; Reinit, go go go!
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    ; Still use the old stack.
    mov sp, 07c00h
    ; Graphices settings
    mov ax, 0B800h
    mov gs, ax
    ; Print "Initing ......"
    call Nextline
    mov ax, InitMessage
    mov cx, InitMessage_len
    call Print
    call Nextline
    ; Copy the kernel to mem.
    call CopyKernelToMem
    call KillMotor
    ; Ok, kernel loaded, jump in protect mode.
    ; Close interrupt
    cli
    ; Load the gdt pointer
    lgdt [GdtPointer]
    ; Load idt.
    lidt [IdtPointer]
    ; Open A20
    in al, 92h
    or al, 2h
    out 92h, al
    ; Work in portect mode.
    mov eax, cr0
    or eax, 01h
    mov cr0, eax
    ; Jump!!
    jmp dword 08h:Protect
    ;
    ; Sleep forever.
    jmp $

;
; Copy the loader from floppy to memory.
;
CopyKernelToMem:
    mov ax, 0240h
    sub al, KernelStartSector
    inc al
    mov bx, End
    mov cx, 6
    mov dx, 0
    int 13h
    ret

;
; Close floppy motor.
;
KillMotor:
    mov ax, 000ch
    mov dx, 03f2h
    out dx, al
    ret

;
; Print a string.
;
; ax : The address of string
; cx : The length of string
;
Print:
    ; Print things by BIOS interrupt No.16
    mov bp, ax
    mov ax, 01301h
    mov bx, 07h
    mov dl, 0
    int 10h
    ret

;
; Move the cursor to the next line.
;
Nextline:
    ; Get cursor
    mov ah, 3
    mov bh, 0
    int 10h
    mov ah, 2
    inc dh
    mov dl, 0
    int 10h
    ret

;
; Variable Table
;
InitMessage db "Initing......"
InitMessage_len equ 13
KernelStartHead equ 0
KernelStartCylinder equ 0
KernelStartSector equ 6
KernelEndHead equ 0
KernelEndCylinder equ 18
KernelEndSector equ 11
KernelLength equ 384

;
; Global Description Table -- GDT
;
GdtStart:
; We don't use first node
dw 0, 0, 0, 0
; Code segment.
dw 0FFFFh, 0h, 9A00h, 0C0h
; Data segment.
dw 0FFFFh, 0h, 9200h, 0C0h
GdtEnd:

;
; Interrupt Descriptor Table -- IDT
;
IdtStart:
%rep 64
dw Empty, 08h, 08E00h, 0h
%endrep
IdtEnd:


GdtPointer:
    dw 18h
    dd GdtStart
IdtPointer:
    dw 256
    dd IdtStart

; 32 bit code here.
[SECTION .s32]
[BITS 32]
Protect:
    ; Init, again and again
    mov ax, 10h
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 07c00h
    ; Now, parse the kernel!
    mov eax, [End]
    push eax
    mov ebx, 0
    mov ecx, End + 4
    mov eax, [ecx]
.Parse:
    cmp ebx, 0
    jz .FindHead
    jnz .Copy
.FindHead:
    mov al, [ecx + 0]
    cmp al, 'S'
    jnz .Ok
    mov al, [ecx + 1]
    cmp al, 'C'
    jnz .Ok
    mov al, [ecx + 2]
    cmp al, 'M'
    jnz .Ok
    mov al, [ecx + 3]
    cmp al, 'D'
    jnz .Ok
    mov edx, [ecx + 4]
    mov ebx, [ecx + 8]
    add ecx, 12
    jmp .Parse
.Copy:
    mov esi, [ecx]
    mov [edx], esi
    add ecx, 1
    add edx, 1
    sub ebx, 1
    jmp .Parse
.Ok:
    ; Sleep forever.
    pop eax
    jmp eax
    jmp $

Empty:
    iretd

End:

;
; How wonderfull the boot program works!
;
; We won't need to worrie about idt, gdt, A20 or
; other things in the kernel init. We can sti anywhere.
; But, today idt is like an empty paper, and gdt is 
; simple. Paging is also closed. In the kernel, we
; must change the gdt&idt to its own, and init 8259A.
; Then, there will used by memory using module.
;
; ^o^ ~~ I can use C to write kernel now ...
;

