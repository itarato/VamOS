#include "disk.h"
#include "interrupt.h"
#include "keyboard.h"
#include "mem.h"
#include "shell.h"
#include "strings.h"
#include "timer.h"
#include "vga.h"

void kernel_main() {
  setup_mem_regions();
  enable_interrupts();

  asm volatile("sti");

  timer_init(400);
  keyboard_init();
  disk_init();

  vga_clear_screen();
  printl("Kernel:\n\tVGA text mode initialized!");
  printl("\tTesting interrupts...");

  asm volatile("int $4");
  // Base CPU interrupts expect an error code - without it SP cannot point to
  // the return addr.
  // asm volatile("int $11");

  shell_init();
}
