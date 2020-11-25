#include "interrupt.h"
#include "keyboard.h"
#include "shell.h"
#include "timer.h"
#include "vga.h"

void main() {
  enable_interrupts();

  __asm__ volatile("sti");

  timer_init(10);
  keyboard_init();

  vga_clear_screen();
  printl("Kernel:\n\tVGA text mode initialized!");
  printl("\tTesting interrupts...");

  __asm__ __volatile__("int $4");
  // Base CPU interrupts expect an error code - without it SP cannot point to
  // the return addr.
  // __asm__ __volatile__("int $11");

  shell_init();
}
