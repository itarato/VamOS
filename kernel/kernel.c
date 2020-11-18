#include "interrupt.h"
#include "vga.h"

void main() {
  enable_interrupts();

  vga_clear_screen();
  vga_printl_on_cursor("Kernel:\n\tVGA text mode initialized!");
  vga_printl_on_cursor("\tTesting interrupts...");

  // __asm__ __volatile__("int $2");
  __asm__ __volatile__("int $9");
}
