#include "interrupt.h"

#include "io.h"
#include "strings.h"
#include "vga.h"

#define PIC_MASTER_CMD 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_CMD 0xA0
#define PIC_SLAVE_DATA 0xA1
#define PIC_ACK 0x20

irq_callback_t irq_handlers[IDT_COUNT];

char* interrupt_names[] = {
    // CPU exceptions (0..31).
    "Division By Zero",
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
    "Reserved",

    // IRQs (32..47).
    "Programmable Interrupt Timer Interrupt",
    "Keyboard Interrupt",
    "Cascade (used internally by the two PICs. never raised)",
    "COM2 (if enabled)",
    "COM1 (if enabled)",
    "LPT2 (if enabled)",
    "Floppy Disk",
    "LPT1 / Unreliable 'spurious' interrupt (usually)",
    "CMOS real-time clock (if enabled)",
    "Free for peripherals / legacy SCSI / NIC",
    "Free for peripherals / SCSI / NIC",
    "Free for peripherals / SCSI / NIC",
    "PS2 Mouse",
    "FPU / Coprocessor / Inter-processor",
    "Primary ATA Hard Disk",
    "Secondary ATA Hard Disk ",
};

void enable_interrupts() {
  set_idt_gate(0, (u32)isr0);
  set_idt_gate(1, (u32)isr1);
  set_idt_gate(2, (u32)isr2);
  set_idt_gate(3, (u32)isr3);
  set_idt_gate(4, (u32)isr4);
  set_idt_gate(5, (u32)isr5);
  set_idt_gate(6, (u32)isr6);
  set_idt_gate(7, (u32)isr7);
  set_idt_gate(8, (u32)isr8);
  set_idt_gate(9, (u32)isr9);
  set_idt_gate(10, (u32)isr10);
  set_idt_gate(11, (u32)isr11);
  set_idt_gate(12, (u32)isr12);
  set_idt_gate(13, (u32)isr13);
  set_idt_gate(14, (u32)isr14);
  set_idt_gate(15, (u32)isr15);
  set_idt_gate(16, (u32)isr16);
  set_idt_gate(17, (u32)isr17);
  set_idt_gate(18, (u32)isr18);
  set_idt_gate(19, (u32)isr19);
  set_idt_gate(20, (u32)isr20);
  set_idt_gate(21, (u32)isr21);
  set_idt_gate(22, (u32)isr22);
  set_idt_gate(23, (u32)isr23);
  set_idt_gate(24, (u32)isr24);
  set_idt_gate(25, (u32)isr25);
  set_idt_gate(26, (u32)isr26);
  set_idt_gate(27, (u32)isr27);
  set_idt_gate(28, (u32)isr28);
  set_idt_gate(29, (u32)isr29);
  set_idt_gate(30, (u32)isr30);
  set_idt_gate(31, (u32)isr31);

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

  set_idt_gate(32, (u32)irq0);
  set_idt_gate(33, (u32)irq1);
  set_idt_gate(34, (u32)irq2);
  set_idt_gate(35, (u32)irq3);
  set_idt_gate(36, (u32)irq4);
  set_idt_gate(37, (u32)irq5);
  set_idt_gate(38, (u32)irq6);
  set_idt_gate(39, (u32)irq7);
  set_idt_gate(40, (u32)irq8);
  set_idt_gate(41, (u32)irq9);
  set_idt_gate(42, (u32)irq10);
  set_idt_gate(43, (u32)irq11);
  set_idt_gate(44, (u32)irq12);
  set_idt_gate(45, (u32)irq13);
  set_idt_gate(46, (u32)irq14);
  set_idt_gate(47, (u32)irq15);

  set_idt_register();
}

void global_isr_handler(int_regs_t* regs) {
  char hexbuf[8];

  print("Interrupt (ISR) called:\n\tID:");
  print(int_to_hex_string(regs->idx, hexbuf, 8));
  print("\n\tERR:");
  print(int_to_hex_string(regs->extra_code, hexbuf, 8));
  print("\n\t");
  printl(interrupt_names[regs->idx]);
}

void irq_ack(u8 isr_no) {
  if (isr_no >= 40) io_byte_out(PIC_SLAVE_CMD, PIC_ACK);
  io_byte_out(PIC_MASTER_CMD, PIC_ACK);
}

void global_irq_handler(int_regs_t* regs) {
  if (irq_handlers[regs->extra_code] != NULL) {
    irq_handlers[regs->extra_code](regs);
  } else {
    char hexbuf[8];

    print("Interrupt (IRQ) called:\n\tID:");
    print(int_to_hex_string(regs->idx, hexbuf, 8));
    print("\n\tIRQ:");
    print(int_to_hex_string(regs->extra_code, hexbuf, 8));
    print("\n\t");
    printl(interrupt_names[regs->idx]);
  }

  irq_ack(regs->idx);
}

void set_idt_gate(u32 idx, u32 handler) {
  idt[idx].offset_lo = (u16)(handler & 0xffff);
  idt[idx].segment_selector = KERNEL_SEGMENT;
  idt[idx].reserved = 0x0;
  idt[idx].flags = 0x8E;
  idt[idx].offset_hi = (u16)((handler >> 16) & 0xffff);
}

void set_idt_register() {
  idt_register.base = (u32)&idt;
  idt_register.limit = IDT_COUNT * sizeof(idt_gate_t) - 1;

  asm volatile("lidtl (%0)" : : "r"(&idt_register));
}

void register_irq_handler(u32 irq_no, irq_callback_t handler) {
  irq_handlers[irq_no] = handler;
}
