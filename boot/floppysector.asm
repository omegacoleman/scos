;
; Ok, BIOS copied us here, 0x7c00
;
    org 07c00h
;
; The main body of the code.
;
    ; Stack must be here.
    Stack:
    ; Just init the ds and es.
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, Stack
    ; Clear the screen.
    mov ax, 0600h
    mov bx, 0700h
    mov cx, 0
    mov dx, 0ffffh
    int 10h
    mov ah, 2
    mov dx, 0
    int 10h
    ; Print out "SCOS 0.01"
    mov ax, OsName
    mov cx, OsName_len
    call Print
    ; Go to the next line
    call Nextline
    call Nextline
    call Nextline
    ; Print out "Booting......"
    mov ax, BootMessage
    mov cx, BootMessage_len
    call Print
    call Nextline
    ; Copy loader to memory, if failed ......
    call CopyLoaderToMem
    ; Then print out the message.
    mov ax, ErrorMessage
    mov cx, ErrorMessage_len
    call Print
    call Nextline
    ; And sleep forever.
    jmp $

;
; Copy the loader from floppy to memory.
;
CopyLoaderToMem:
    mov dx, 0h
    mov cx, 02h
    mov bx, End
    mov ax, 0200h + LoaderLength
    int 13h
    jnc End
    ; If failed, return.
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
OsName db "SCOS "
db ver
OsName_len equ ($ - OsName)
BootMessage db "Booting......"
BootMessage_len equ 13
ErrorMessage db "Boot failed! Sleeping forever."
ErrorMessage_len equ 30
LoaderLength equ 4

; Grow the file fill the sector.
times 510-($-$$) db 0

; Ending sign of the boot sector
dw 0aa55h

End:
