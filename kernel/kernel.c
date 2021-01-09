#include "disk.h"
#include "interrupt.h"
#include "keyboard.h"
#include "mem.h"
#include "shell.h"
#include "strings.h"
#include "timer.h"
#include "vga.h"

typedef struct {
  u32 pte[1024];
} __attribute__((packed)) page_table_t;

typedef struct {
  u32 pde[1024];
} __attribute__((packed, aligned(4096))) page_directory_t;
static page_directory_t pd;

void kernel_main() {
  vga_clear_screen();
  printl("[kernel] VGA text mode initialized");

  setup_mem_regions();
  printl("[kernel] memory regions established");

  enable_interrupts();
  asm volatile("sti");
  printl("[kernel] interrupts enabled");
  // Interrupt example.
  // printl("\tTesting interrupts...");
  // asm volatile("int $4");
  // Base CPU interrupts expect an error code - without it SP cannot point to
  // the return addr.
  // asm volatile("int $11");

  timer_init(400);
  printl("[kernel] timer initialized");

  keyboard_init();
  printl("[kernel] keyboard initialized");

  disk_init();
  // Disk read write example.
  // u8* buf = malloc(512);
  // disk_read(0, 0, 1, 0, buf);
  // for (int i = 0; i < 512; i++) buf[i] = 0xab;
  // disk_write(0, 0, 1, 0, buf);
  printl("[kernel] disk initialized");

  printl("[kernel] shell init");
  shell_init();
}
