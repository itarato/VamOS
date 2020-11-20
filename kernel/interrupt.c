#include "interrupt.h"

#include "io.h"
#include "vga.h"

#define PIC_MASTER_CMD 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CMD 0xA0
#define PIC_SLAVE_DATA 0xA1

char *interrupt_names[] = {"Division By Zero",
                           "Debug",
                           "Non Maskable Interrupt",
                           "Breakpoint",
                           "Into Detected Overflow",
                           "Out of Bounds",
                           "Invalid Opcode",
                           "No Coprocessor",

                           "Double Fault",
                           "Coprocessor Segment Overrun",
                           "Bad TSS",
                           "Segment Not Present",
                           "Stack Fault",
                           "General Protection Fault",
                           "Page Fault",
                           "Unknown Interrupt",

                           "Coprocessor Fault",
                           "Alignment Check",
                           "Machine Check",
                           "Reserved",
                           "Reserved",
                           "Reserved",
                           "Reserved",
                           "Reserved",

                           "Reserved",
                           "Reserved",
                           "Reserved",
                           "Reserved",
                           "Reserved",
                           "Reserved",
                           "Reserved",
                           "Reserved"};

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

  io_byte_out(PIC_MASTER_CMD, 0x11);
  io_byte_out(PIC_SLAVE_CMD, 0x11);
  io_byte_out(PIC_MASTER_DATA, 0x20);
  io_byte_out(PIC_SLAVE_DATA, 0x28);
  io_byte_out(PIC_MASTER_DATA, 0x4);
  io_byte_out(PIC_SLAVE_DATA, 0x2);
  io_byte_out(PIC_MASTER_DATA, 0x1);
  io_byte_out(PIC_SLAVE_DATA, 0x1);
  io_byte_out(PIC_MASTER_DATA, 0x0);
  io_byte_out(PIC_SLAVE_DATA, 0x0);

  set_idt_gate(32, (uint_t)irq0);
  set_idt_gate(33, (uint_t)irq1);
  set_idt_gate(34, (uint_t)irq2);
  set_idt_gate(35, (uint_t)irq3);
  set_idt_gate(36, (uint_t)irq4);
  set_idt_gate(37, (uint_t)irq5);
  set_idt_gate(38, (uint_t)irq6);
  set_idt_gate(39, (uint_t)irq7);
  set_idt_gate(40, (uint_t)irq8);
  set_idt_gate(41, (uint_t)irq9);
  set_idt_gate(42, (uint_t)irq10);
  set_idt_gate(43, (uint_t)irq11);
  set_idt_gate(44, (uint_t)irq12);
  set_idt_gate(45, (uint_t)irq13);
  set_idt_gate(46, (uint_t)irq14);
  set_idt_gate(47, (uint_t)irq15);

  set_idt_register();
}

void global_isr_handler(isr_call_stack_t regs) {
  vga_print_on_cursor("Interrupt (ISR) called:\n\tID:");
  vga_print_hex(regs.idx);
  vga_print_on_cursor("\n\tERR:");
  vga_print_hex(regs.err_no);
  vga_print_on_cursor("\n\t");
  vga_printl_on_cursor(interrupt_names[regs.idx]);
}

void global_irq_handler(isr_call_stack_t regs) {
  vga_print_on_cursor("Interrupt (IRQ) called:\n\tID:");
  vga_print_hex(regs.idx);
  vga_print_on_cursor("\n\tIRQ:");
  vga_print_hex(regs.err_no);
  vga_print_on_cursor("\n\t");
  vga_printl_on_cursor(interrupt_names[regs.idx]);

  if (regs.idx >= 40) io_byte_out(PIC_SLAVE_CMD, 0x20);
  io_byte_out(PIC_MASTER_CMD, 0x20);
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
