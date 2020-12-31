; First char PTR into bx.
print:
    pusha
    mov ah, 0x0e
    
    .print_loop:
        mov al, [bx]
        cmp al, 0x0
        je .print_end

        int 0x10

        inc bx
        jmp .print_loop

    .print_end:
        popa
        ret
