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

    mov si, kernel_copy_gdt
    mov al, 0
    mov ah, 0x87
    mov ecx, 10000
    int 0x15
    jc .err
    ret

    .err:
        mov bx, err_msg
        call print
        jmp $


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


boot_msg: db 'RMd', 10, 13, 0
boot_pm_msg: db 'PMd', 0
load_kernel_msg: db 'Kernel', 10, 13, 0
err_msg: db 'ERR', 10, 13, 0
boot_drive: db 0

; https://forum.osdev.org/viewtopic.php?p=314362#p314362
kernel_copy_gdt:
    dq 0 ; 00  8bytes	dummy, set to 0
    dq 0 ; 08  8bytes	GDT data segment location, set to 0
    
    ; 10  8bytes	source GDT pointer
    ; This is safe for now because kernel is small.
    dw 20000
    db 0, 0x10, 0
    db 0x93
    dw 0
    
    ; 18  8bytes	target GDT pointer
    dw 20000
    db 0, 0, 0x10
    db 0x93

    dw 0    ; 20  8bytes	pointer to BIOS code segment, set to 0.	used by
            ;               the BIOS to create protected mode code segment

    dq 0, 0 ; 28  8bytes	pointer to BIOS stack segment, set to 0.  Used by
            ;               the BIOS to create protected mode stack segment
    dq 0, 0 ; 30  8bytes	user code segment
    dq 0, 0 ; 38  8bytes	temporary BIOS code segment

times 510-($-$$) db 0
dw 0xaa55
