#include "interrupt.h"
#include "keyboard.h"
#include "shell.h"
#include "strings.h"
#include "timer.h"
#include "vga.h"

void kernel_main(u16 memsize) {
  enable_interrupts();

  asm volatile("sti");

  timer_init(10);
  keyboard_init();

  vga_clear_screen();
  printl("Kernel:\n\tVGA text mode initialized!");
  printl("\tTesting interrupts...");

  char hexbuf[9];
  int_to_hex_string(memsize, hexbuf, 9);
  printl(hexbuf);

  asm volatile("int $4");
  // Base CPU interrupts expect an error code - without it SP cannot point to
  // the return addr.
  // asm volatile("int $11");

  shell_init();
}
