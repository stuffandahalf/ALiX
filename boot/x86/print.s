print:
    pushw %ax
    movb $0x0e, %ah
1:
    lodsb
    testb %al, %al
    jz 2f
    int $0x10
    jmp 1b
2:
    popw %ax
    ret

