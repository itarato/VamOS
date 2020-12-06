#include "interrupt.h"
#include "keyboard.h"
#include "shell.h"
#include "timer.h"
#include "vga.h"

void kernel_main() {
  enable_interrupts();

  asm volatile("sti");

  timer_init(10);
  keyboard_init();

  vga_clear_screen();
  printl("Kernel:\n\tVGA text mode initialized!");
  printl("\tTesting interrupts...");

  asm volatile("int $4");
  // Base CPU interrupts expect an error code - without it SP cannot point to
  // the return addr.
  // asm volatile("int $11");

  shell_init();
}
