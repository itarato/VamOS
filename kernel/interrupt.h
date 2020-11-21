#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "types.h"

#define KERNEL_SEGMENT 0x8
#define IDT_COUNT 0x100

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

typedef struct {
  u32 ds;

  u32 edi;
  u32 esi;
  u32 ebp;
  u32 esp;
  u32 ebx;
  u32 edx;
  u32 ecx;
  u32 eax;

  u32 idx;
  // Error code for CPU exceptions and IRQ number for IRQ.
  u32 extra_code;

  u32 eip;
  u32 cs;
  u32 eflags;
  u32 usersp;
  u32 ss;
} int_regs_t;  // TODO Rename this type.

void enable_interrupts();
void global_isr_handler(int_regs_t regs);
void global_irq_handler(int_regs_t regs);

typedef struct {
  u16 offset_lo;
  u16 segment_selector;
  u8 reserved;
  u8 flags;
  u16 offset_hi;
} __attribute__((packed)) idt_gate_t;

typedef struct {
  u16 limit;
  u32 base;
} __attribute__((packed)) idt_register_t;

idt_gate_t idt[IDT_COUNT];
idt_register_t idt_register;

void set_idt_gate(u32 idx, u32 handler);
void set_idt_register();

typedef void (*irq_callback_t)(int_regs_t);
void register_irq_handler(u32 irq_no, irq_callback_t handler);

#endif  // INTERRUPT_H
