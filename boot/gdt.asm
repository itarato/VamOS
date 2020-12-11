; https://wiki.osdev.org/Global_Descriptor_Table

gdt_start:
    dd 0x0
    dd 0x0

gdt_code:
    ; Base: 0
    ; Limit (length): 0xfffff

    ; Limit 0:15
    dw 0xffff
    ; Base 0:15
    dw 0x0
    ; Base 16:23
    db 0x0
    ; Access byte
    ; (7) Present bit. This must be 1 for all valid selectors. -> 1
    ; (6-5) Privilege, 2 bits. Contains the ring level, 0 = highest (kernel), 3 = lowest (user applications). -> 0
    ; (4) Descriptor type. This bit should be set for code or data segments and should be cleared for system segments 
    ;     (eg. a Task State Segment) -> 1
    ; (3) Executable bit. If 1 code in this segment can be executed, ie. a code selector. If 0 it is a data selector. -> 1
    ; (2) Direction bit/Conforming bit. -> 0
    ; (1) Readable bit/Writable bit. -> 1
    ; (0) Accessed bit. Just set to 0. The CPU sets this to 1 when the segment is accessed. -> 0
    db 10011010b
    ; Flags + limit
    ; Flags:
    ; - (7) Granularity bit. If 0 the limit is in 1 B blocks (byte granularity), 
    ;   if 1 the limit is in 4 KiB blocks (page granularity). -> 1
    ; - (6) Size bit. If 0 the selector defines 16 bit protected mode. If 1 it defines 32 bit protected mode. 
    ;   You can have both 16 bit and 32 bit selectors at once. -> 1
    db 11001111b
    ; Base 24:31
    db 0x0

gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
