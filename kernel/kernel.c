#include "vga.h"

void main() {
  vga_clear_screen();
  vga_print_on_cursor("VGA text mode initialized!");
  vga_new_line();
  vga_print_on_cursor("Waiting...");
}
