disk_load:
    pusha

    push dx

    ; https://stanislavs.org/helppc/int_13-2.html
    mov ah, 0x2
    mov al, dh
    mov cl, 0x2
    mov ch, 0x0
    mov dh, 0x0

    int 0x13
    jc disk_load_error

    pop dx
    cmp al, dh
    jne disk_load_sector_error

    popa
    ret


disk_load_error:
    mov bx, disk_load_error_msg
    call print
    jmp $


disk_load_sector_error:
    mov bx, disk_load_sector_error_msg
    call print
    jmp $


disk_load_error_msg: db 'Disk error!', 0x0
disk_load_sector_error_msg: db 'Sector error!', 0x0
