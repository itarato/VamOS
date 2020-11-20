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
  uint_t ds;

  uint_t edi;
  uint_t esi;
  uint_t ebp;
  uint_t esp;
  uint_t ebx;
  uint_t edx;
  uint_t ecx;
  uint_t eax;

  uint_t idx;
  uint_t err_no;

  uint_t eip;
  uint_t cs;
  uint_t eflags;
  uint_t usersp;
  uint_t ss;
} isr_call_stack_t;

void enable_interrupts();
void global_isr_handler(isr_call_stack_t regs);
void global_irq_handler(isr_call_stack_t regs);

typedef struct {
  word_t offset_lo;
  word_t segment_selector;
  byte_t reserved;
  byte_t flags;
  word_t offset_hi;
} __attribute__((packed)) idt_gate_t;

typedef struct {
  word_t limit;
  uint_t base;
} __attribute__((packed)) idt_register_t;

idt_gate_t idt[IDT_COUNT];
idt_register_t idt_register;

void set_idt_gate(unsigned int idx, uint_t handler);
void set_idt_register();

#endif  // INTERRUPT_H
