[org 0x7c00]
KERNEL_OFFS equ 0x1000

mov [boot_drive], dl

mov bp, 0x9000
mov sp, bp

mov bx, boot_msg
call print

call load_kernel

call load_memory_map

call switch_to_protected_mode

jmp $

%include "boot/util.asm"
%include "boot/util_pm.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/pm_switch.asm"

[bits 16]
load_memory_map:
    clc
    int 0x12
    jc load_memory_map_error
    mov [mem_size], ax
    ret
load_memory_map_error:
    mov bx, memory_map_error_msg
    call print
    jmp $

[bits 16]
load_kernel:
    mov bx, load_kernel_msg
    call print

    mov bx, KERNEL_OFFS
    mov dh, 32
    mov dl, [boot_drive]
    call disk_load

    ret

[bits 32]
start_protected_mode:
    mov ebx, boot_pm_msg
    call print_pm

    ; Set start of arg address.
    mov bx, mem_size
    call KERNEL_OFFS

    jmp $

boot_msg: db 'Start 16bit real mode', 0x0
boot_pm_msg: db 'Start 32bit protected mode', 0x0
memory_map_error_msg: db 'Memory map error', 0x0
load_kernel_msg: db 'Loading kernel.', 0x0

boot_drive: db 0x00

; Args to kernel:
mem_size: dw 0x0000

times 510-($-$$) db 0
dw 0xaa55
