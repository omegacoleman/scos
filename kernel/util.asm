global outbyte
global inbyte
global outword
global inword
global disableint
global enableint

[SECTION .text]

;
; Out byte to port.
;
outbyte:
    push ebp
    mov ebp, esp
    push edx
    push eax
    mov edx, [ebp + 8]
    mov al, [ebp + 12]
    out dx, al
    nop
    nop
    pop eax
    pop edx
    pop ebp
    ret

;
; Read byte from port.
;
inbyte:
    push ebp
    mov ebp, esp
    push edx
    xor eax, eax
    mov edx, [ebp + 8]
    in al, dx
    nop
    nop
    pop edx
    pop ebp
    ret

;
; Out word to port.
;
outword:
    push ebp
    mov ebp, esp
    push edx
    push eax
    mov edx, [ebp + 8]
    mov ax, [ebp + 12]
    out dx, ax
    nop
    nop
    pop eax
    pop edx
    pop ebp
    ret

;
; Read word from port.
;
inword:
    push ebp
    mov ebp, esp
    push edx
    xor eax, eax
    mov edx, [ebp + 8]
    in ax, dx
    nop
    nop
    pop edx
    pop ebp
    ret

;
; Set IF to 0.
;
disableint:
    cli
    ret

;
; Set IF to 1.
;
enableint:
    sti
    ret

