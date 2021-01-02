[bits 32]
[extern kernel_main]

PAGE_DIR_LOC equ 0x200000
PAGE_TABLE_1_LOC equ 0x201000

; Clear PD.
mov edi, PAGE_DIR_LOC
mov ecx, 0x400
xor eax, eax
rep stosd

; Set 1st PT.
mov ecx, 0
page_table_setup_loop:
  mov eax, 0x1000
  mul cx
  or eax, 0b000000010011
  mov [PAGE_TABLE_1_LOC + ecx * 4], eax

  inc ecx
  cmp ecx, 1024
  jne page_table_setup_loop

; Set 1st PT in PD.
mov eax, PAGE_TABLE_1_LOC
and eax, 0xFFFFF000
or eax, 0b000000010011
mov [PAGE_DIR_LOC], eax

call kernel_main

jmp $
