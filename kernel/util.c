#include "util.h"

#include "vga.h"

void panic(char *msg) {
  vga_print_on_cursor("PANIC: ");
  vga_printl_on_cursor(msg);
}
