[extern global_isr_handler]
[extern global_irq_handler]

KERNEL_DATA_SEGMENT equ 0x10

; Stack top: interrupt idx (top), error code.
isr_generic_interrupt_wrapper:
    pusha

    mov ax, ds
    push eax
    mov ax, KERNEL_DATA_SEGMENT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    cld
    call global_isr_handler

    pop eax
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    popa
    add esp, 0x8
    iret

irq_generic_interrupt_wrapper:
    pusha
    
    mov ax, ds
    push eax
    mov ax, KERNEL_DATA_SEGMENT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    push esp
    cld
    call global_irq_handler

    pop ebx
    pop ebx
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx

    popa
    add esp, 8
    iret

global isr0
global isr1
global isr2
global isr3
global isr4
global isr5
global isr6
global isr7
global isr8
global isr9
global isr10
global isr11
global isr12
global isr13
global isr14
global isr15
global isr16
global isr17
global isr18
global isr19
global isr20
global isr21
global isr22
global isr23
global isr24
global isr25
global isr26
global isr27
global isr28
global isr29
global isr30
global isr31

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15

isr0:
    push byte 0
    push byte 0
    jmp isr_generic_interrupt_wrapper

isr1:
    push byte 0
    push byte 1
    jmp isr_generic_interrupt_wrapper

isr2:
    push byte 0
    push byte 2
    jmp isr_generic_interrupt_wrapper

isr3:
    push byte 0
    push byte 3
    jmp isr_generic_interrupt_wrapper

isr4:
    push byte 0
    push byte 4
    jmp isr_generic_interrupt_wrapper

isr5:
    push byte 0
    push byte 5
    jmp isr_generic_interrupt_wrapper

isr6:
    push byte 0
    push byte 6
    jmp isr_generic_interrupt_wrapper

isr7:
    push byte 0
    push byte 7
    jmp isr_generic_interrupt_wrapper

isr8:
    push byte 8
    jmp isr_generic_interrupt_wrapper

isr9:
    push byte 0
    push byte 9
    jmp isr_generic_interrupt_wrapper

isr10:
    push byte 10
    jmp isr_generic_interrupt_wrapper

isr11:
    push byte 11
    jmp isr_generic_interrupt_wrapper

isr12:
    push byte 12
    jmp isr_generic_interrupt_wrapper

isr13:
    push byte 13
    jmp isr_generic_interrupt_wrapper

isr14:
    push byte 14
    jmp isr_generic_interrupt_wrapper

isr15:
    push byte 0
    push byte 15
    jmp isr_generic_interrupt_wrapper

isr16:
    push byte 0
    push byte 16
    jmp isr_generic_interrupt_wrapper

isr17:
    push byte 17
    jmp isr_generic_interrupt_wrapper

isr18:
    push byte 0
    push byte 18
    jmp isr_generic_interrupt_wrapper

isr19:
    push byte 0
    push byte 19
    jmp isr_generic_interrupt_wrapper

isr20:
    push byte 0
    push byte 20
    jmp isr_generic_interrupt_wrapper

isr21:
    push byte 0
    push byte 21
    jmp isr_generic_interrupt_wrapper

isr22:
    push byte 0
    push byte 22
    jmp isr_generic_interrupt_wrapper

isr23:
    push byte 0
    push byte 23
    jmp isr_generic_interrupt_wrapper

isr24:
    push byte 0
    push byte 24
    jmp isr_generic_interrupt_wrapper

isr25:
    push byte 0
    push byte 25
    jmp isr_generic_interrupt_wrapper

isr26:
    push byte 0
    push byte 26
    jmp isr_generic_interrupt_wrapper

isr27:
    push byte 0
    push byte 27
    jmp isr_generic_interrupt_wrapper

isr28:
    push byte 0
    push byte 28
    jmp isr_generic_interrupt_wrapper

isr29:
    push byte 0
    push byte 29
    jmp isr_generic_interrupt_wrapper

isr30:
    push byte 0
    push byte 30
    jmp isr_generic_interrupt_wrapper

isr31:
    push byte 0
    push byte 31
    jmp isr_generic_interrupt_wrapper


irq0:
    push byte 0
    push byte 32
    jmp irq_generic_interrupt_wrapper

irq1:
    push byte 1
    push byte 33
    jmp irq_generic_interrupt_wrapper

irq2:
    push byte 2
    push byte 34
    jmp irq_generic_interrupt_wrapper

irq3:
    push byte 3
    push byte 35
    jmp irq_generic_interrupt_wrapper

irq4:
    push byte 4
    push byte 36
    jmp irq_generic_interrupt_wrapper

irq5:
    push byte 5
    push byte 37
    jmp irq_generic_interrupt_wrapper

irq6:
    push byte 6
    push byte 38
    jmp irq_generic_interrupt_wrapper

irq7:
    push byte 7
    push byte 39
    jmp irq_generic_interrupt_wrapper

irq8:
    push byte 8
    push byte 40
    jmp irq_generic_interrupt_wrapper

irq9:
    push byte 9
    push byte 41
    jmp irq_generic_interrupt_wrapper

irq10:
    push byte 10
    push byte 42
    jmp irq_generic_interrupt_wrapper

irq11:
    push byte 11
    push byte 43
    jmp irq_generic_interrupt_wrapper

irq12:
    push byte 12
    push byte 44
    jmp irq_generic_interrupt_wrapper

irq13:
    push byte 13
    push byte 45
    jmp irq_generic_interrupt_wrapper

irq14:
    push byte 14
    push byte 46
    jmp irq_generic_interrupt_wrapper

irq15:
    push byte 15
    push byte 47
    jmp irq_generic_interrupt_wrapper
