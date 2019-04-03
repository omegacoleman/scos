global keyboardint
global keymap
global statekeys
global controls
global locks

extern keyboardinterrupted

[SECTION .text]

zpg dd 0
keyboardint:
    pushad
    mov eax, cr3
    mov [zpg], eax
    mov eax, 0x200000
    mov cr3, eax
    mov al, 020h
    out 0x20, al
    xor eax, eax
    in al, 0x60
    push eax
    call keyboardinterrupted
    add esp, 4
    mov eax, [zpg]
    mov cr3, eax
    popad
    iretd

keymap:
    db 0, 0
    db "1234567890"
    db 0, 0, 8, 0
    db "qwertyuiop[]"
    db 0xa, 0
    db "asdfghjkl;'`"
    db 0
    db "\zxcvbnm,./"
    db 0, 0, 0, " "
    times (0xff - ($ - keymap)) db 0
.shift:
    db 0, 0
    db "!@#$%^&*()"
    db 0, 0, 0, 0
    db "QWERTYUIOP{}"
    db 0, 0
    db "ASDFGHJKL:", '"'
    db 0, 0
    db "|ZXCVBNM<>?"
    db 0, 0, 0, " "
    times (0xff - ($ - .shift)) db 0

statekeys:
controls:
    db 0x2a ; shift
    db 0x1d ; ctrl
    db 0x38 ; alt
locks:
    db 0x3a ; caps lock
    db 0x45 ; num lock
    db 0x46 ; scroll lock (unsupport)

