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

  // // Clean PD.
  // for (int i = 0; i < 1024; i++) {
  //   pd.pde[i] = 0;
  // }
  // // Enable paging -> the very first PDE.
  // // https://wiki.osdev.org/Paging
  // page_table_t* pt1 = malloc(0x1000, 0x1000);
  // for (int i = 0; i < 1024; i++) {
  //   pt1->pte[i] = ((0x1000 * i) & ~0xFFF) | 0b000000010011;
  // }
  // pd.pde[0] = ((u32)pt1 & ~0xFFF) | 0b000000010011;

  // asm volatile("mov %%eax, %%cr3" ::"a"(&pd));
  // asm volatile("mov %cr0, %eax");
  // // https://en.wikipedia.org/wiki/Control_register
  // asm volatile("or $0xC0000001, %eax");
  // asm volatile("mov %eax, %cr0");
  printl("[kernel] paging enabled");

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
