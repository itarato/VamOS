; First char PTR into bx.
print:
    pusha

    mov ah, 0x0e

print_loop:
    mov al, [bx]
    cmp al, 0x0
    je print_end

    int 0x10

    inc bx
    jmp print_loop

print_end:

    call print_new_line

    popa
    ret
    

print_new_line:
    pusha
    mov ah, 0x0e
    mov al, 0xa
    int 0x10
    mov al, 0xd
    int 0x10
    popa
    ret


; Addr to bx.
print_hex:
    pusha

    mov ah, 0x0e

    mov al, '0'
    int 0x10
    mov al, 'x'
    int 0x10

    mov cx, 4

print_hex_loop:

    rol bx, 4

    mov dx, bx
    and dx, 0xf
    mov al, dl
    add al, 0x30

    cmp al, 0x39
    jle print_hex_skip_num_adjust

    add al, 0x7

print_hex_skip_num_adjust:

    int 0x10

    dec cx
    cmp cx, 0
    jg print_hex_loop

    call print_new_line

    popa
    ret
