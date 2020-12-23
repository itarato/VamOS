#include "disk.h"
#include "interrupt.h"
#include "keyboard.h"
#include "mem.h"
#include "shell.h"
#include "strings.h"
#include "timer.h"
#include "vga.h"

void kernel_main() {
  vga_clear_screen();

  setup_mem_regions();
  enable_interrupts();

  asm volatile("sti");

  timer_init(400);
  keyboard_init();
  disk_init();

  printl("Kernel:\n\tVGA text mode initialized!");

  // Interrupt example.
  // printl("\tTesting interrupts...");
  // asm volatile("int $4");
  // Base CPU interrupts expect an error code - without it SP cannot point to
  // the return addr.
  // asm volatile("int $11");

  shell_init();

  // Disk read write example.
  // u8* buf = malloc(512);
  // disk_read(0, 0, 1, 0, buf);
  // for (int i = 0; i < 512; i++) buf[i] = 0xab;
  // disk_write(0, 0, 1, 0, buf);
}
