
global undefined
global nint

[SECTION .text]

; For debug :)
;       -- they are bugs, too.
undefined:
    ud2
    ret

nint:
    int 0x56
    ret
