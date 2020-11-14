[bits 32]

VIDEO_MEM equ 0xb8000
WHITE_ON_BLACK equ 0x0f

; First char ptr to ebx.
print_pm:
    pusha
    mov edx, VIDEO_MEM

print_pm_loop:
    mov al, [ebx]
    cmp al, 0
    je print_pm_end

    mov ah, WHITE_ON_BLACK
    mov [edx], ax
    inc ebx
    add edx, 2
    jmp print_pm_loop

print_pm_end:
    popa
    ret
