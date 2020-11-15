#include "vga.h"

void main() {
  vga_clear_screen();
  vga_printl_on_cursor("Kernel:\n\tVGA text mode initialized!");
  vga_printl_on_cursor("\tWaiting...");
}
