[org 0x7c00]
KERNEL_OFFS equ 0x1000
MEM_REGION_DATA equ 0x8000
SMAP equ 0x0534d4150
MEM_REQ_CODE equ 0xe820

mov [boot_drive], dl

call load_memory_regions

mov bp, 0x9000
mov sp, bp

mov bx, boot_msg
call print

call load_kernel

call switch_to_protected_mode

jmp $

%include "boot/util.asm"
%include "boot/util_pm.asm"
%include "boot/disk.asm"
%include "boot/gdt.asm"
%include "boot/pm_switch.asm"

[bits 16]
load_kernel:
    mov bx, load_kernel_msg
    call print

    mov bx, KERNEL_OFFS
    mov dh, 48
    mov dl, [boot_drive]
    call disk_load

    ret

[bits 16]
; https://wiki.osdev.org/Detecting_Memory_(x86)
load_memory_regions:
    mov di, MEM_REGION_DATA + 4
    xor ebx, ebx
    xor bp, bp
    mov edx, SMAP
    mov eax, MEM_REQ_CODE
    mov [es:di + 20], dword 1
    mov ecx, 24
    int 0x15
    jc short .load_memory_regions_failed
    mov edx, SMAP
    cmp eax, edx
    jne short .load_memory_regions_failed
    test ebx, ebx
    je short .load_memory_regions_failed
    jmp short .load_memory_regions_process
.load_memory_regions_loop:
    mov eax, MEM_REQ_CODE
    mov [es:di + 20], dword 1
    mov ecx, 24
    int 0x15
    jc short .load_memory_regions_loop_failed
    mov edx, SMAP
.load_memory_regions_process:
    jcxz .load_memory_regions_skip
    cmp cl, 20
    jbe short .load_memory_regions_ignore
    test byte [es:di + 20], 1
    je short .load_memory_regions_skip
.load_memory_regions_ignore:
    mov ecx, [es:di + 8]
    or ecx, [es:di + 12]
    jz .load_memory_regions_skip
    inc bp
    add di, 24
.load_memory_regions_skip:
    test ebx, ebx
    jne short .load_memory_regions_loop
.load_memory_regions_loop_failed:
    mov [MEM_REGION_DATA], bp
    clc
    ret
.load_memory_regions_failed:
    stc
    ret

[bits 32]
start_protected_mode:
    mov ebx, boot_pm_msg
    call print_pm

    call KERNEL_OFFS

    jmp $

boot_msg: db '16bit real mode', 0x0
boot_pm_msg: db '32bit protected mode', 0x0
load_kernel_msg: db 'Loading kernel.', 0x0
boot_drive: db 0x00

times 510-($-$$) db 0
dw 0xaa55
