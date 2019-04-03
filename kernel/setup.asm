;
; Setup the kernel here.
;
global _start
extern scos
[SECTION .text]
[BITS 32]
; The entry of the kernel
_start:
    ; Reinit ...
    ; The GDT now:
    ; 08h : Callable Segment
    ; 10h : Data Segment
    mov ax, 10h
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov esp, 07c00h
    ; Go!
    call scos
    jmp $
