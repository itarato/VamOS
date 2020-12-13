[bits 32]
[extern kernel_main]

mov dx, [bx + 0]
push dx
call kernel_main

jmp $
