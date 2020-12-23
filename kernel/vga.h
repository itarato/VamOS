#include "types.h"

struct coord_t {
  u32 x;
  u32 y;
};

u32 vga_get_cursor();
struct coord_t vga_get_cursor_coord();

void print(char* s);
void printc(char c);
void printl(char* s);
void printhex(u32 v);
void printf(const char* tpl, ...);
void vga_new_line();

void vga_clear_screen();

void vga_set_cursor(u32 y, u32 x);
void vga_set_cursor_to_offs(u32 offs);
void vga_scroll();
