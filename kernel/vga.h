#include "types.h"

struct coord_t {
  uint_t x;
  uint_t y;
};

uint_t vga_get_cursor();
struct coord_t vga_get_cursor_coord();

void vga_print_on_cursor(char* s);
void vga_new_line();

void vga_clear_screen();

void vga_set_cursor(uint_t y, uint_t x);
void vga_set_cursor_to_offs(uint_t offs);
