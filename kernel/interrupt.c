#include "interrupt.h"

#include "vga.h"

void enable_interrupts() {
  set_idt_gate(0, (uint_t)isr0);
  set_idt_gate(1, (uint_t)isr1);
  set_idt_gate(2, (uint_t)isr2);
  set_idt_gate(3, (uint_t)isr3);
  set_idt_gate(4, (uint_t)isr4);
  set_idt_gate(5, (uint_t)isr5);
  set_idt_gate(6, (uint_t)isr6);
  set_idt_gate(7, (uint_t)isr7);
  set_idt_gate(8, (uint_t)isr8);
  set_idt_gate(9, (uint_t)isr9);
  set_idt_gate(10, (uint_t)isr10);
  set_idt_gate(11, (uint_t)isr11);
  set_idt_gate(12, (uint_t)isr12);
  set_idt_gate(13, (uint_t)isr13);
  set_idt_gate(14, (uint_t)isr14);
  set_idt_gate(15, (uint_t)isr15);
  set_idt_gate(16, (uint_t)isr16);
  set_idt_gate(17, (uint_t)isr17);
  set_idt_gate(18, (uint_t)isr18);
  set_idt_gate(19, (uint_t)isr19);
  set_idt_gate(20, (uint_t)isr20);
  set_idt_gate(21, (uint_t)isr21);
  set_idt_gate(22, (uint_t)isr22);
  set_idt_gate(23, (uint_t)isr23);
  set_idt_gate(24, (uint_t)isr24);
  set_idt_gate(25, (uint_t)isr25);
  set_idt_gate(26, (uint_t)isr26);
  set_idt_gate(27, (uint_t)isr27);
  set_idt_gate(28, (uint_t)isr28);
  set_idt_gate(29, (uint_t)isr29);
  set_idt_gate(30, (uint_t)isr30);
  set_idt_gate(31, (uint_t)isr31);

  set_idt_register();
}

void global_isr_handler(isr_call_stack_t regs) {
  vga_printl_on_cursor("Interrupt called");
  //   vga_new_line();
}

void set_idt_gate(unsigned int idx, uint_t handler) {
  idt[idx].offset_lo = (word_t)(handler & 0xffff);
  idt[idx].segment_selector = KERNEL_SEGMENT;
  idt[idx].reserved = 0x0;
  idt[idx].flags = 0x8E;
  idt[idx].offset_hi = (word_t)((handler >> 16) & 0xffff);
}

void set_idt_register() {
  idt_register.base = (uint_t)&idt;
  idt_register.limit = IDT_COUNT * sizeof(idt_gate_t) - 1;
  __asm__ __volatile__("lidtl (%0)" : : "r"(&idt_register));
}
