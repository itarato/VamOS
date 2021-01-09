[bits 32]
[extern kernel_main]

PAGE_DIR_LOC equ 0x200000
PAGE_TABLE_0x300_LOC equ 0x201000

; https://wiki.osdev.org/Paging
; Clear PD.
mov edi, PAGE_DIR_LOC
mov ecx, 0x400
xor eax, eax
rep stosd

; Set 1st PT.
mov ecx, 0
page_table_setup_loop:
  mov eax, 0x1000
  mul ecx
  or eax, 0b000000010011
  mov [PAGE_TABLE_0x300_LOC + ecx * 4], eax

  inc ecx
  cmp ecx, 1024
  jne page_table_setup_loop

; Set 1st PT in PD.
mov eax, PAGE_TABLE_0x300_LOC
and eax, 0xFFFFF000
or eax, 0b000000010011
mov [PAGE_DIR_LOC + 4 * 0x0], eax
mov [PAGE_DIR_LOC + 4 * 0x300], eax

; Store the virtual kernel start address.
lea edx, kernel_main

; Enable paging.
mov eax, PAGE_DIR_LOC
mov cr3, eax
mov eax, cr0
; https://en.wikipedia.org/wiki/Control_register
or eax, 0xC0000001
mov cr0, eax

; Artificially set return address (to: jmp $).
push dword $ + 7
jmp edx

jmp $
