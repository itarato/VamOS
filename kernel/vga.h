#include "types.h"

struct coord_t {
  u32 x;
  u32 y;
};

u32 vga_get_cursor();
struct coord_t vga_get_cursor_coord();

void print(char* s);
void printl(char* s);
void vga_new_line();

void vga_clear_screen();

void vga_set_cursor(u32 y, u32 x);
void vga_set_cursor_to_offs(u32 offs);
void vga_scroll();
