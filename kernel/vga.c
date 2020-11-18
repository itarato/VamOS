#include "vga.h"

#include "io.h"
#include "mem.h"

#define VGA_MEM_ADDR 0xb8000
#define VGA_CTRL 0x3d4
#define VGA_DATA 0x3d5
#define CURSOR_POS_HI 0xe
#define CURSOR_POS_LO 0xf

#define TEXT_BLACK_ON_WHITE 0xf

#define MODE_7_WIDTH 80
#define MODE_7_HEIGHT 25
#define MODE_7_TEXT_OFFS_LIM 2000

uint_t vga_get_cursor() {
  io_byte_out(VGA_CTRL, CURSOR_POS_HI);
  uint_t pos = io_byte_in(VGA_DATA);
  pos <<= 8;

  io_byte_out(VGA_CTRL, CURSOR_POS_LO);
  pos |= io_byte_in(VGA_DATA);

  return pos;
}

struct coord_t vga_get_cursor_coord() {
  uint_t pos = vga_get_cursor();
  struct coord_t coord;

  coord.x = pos % 80;
  coord.y = pos / 80;

  return coord;
}

// TODO: Print format (numbers).
// TODO: Special chars.
void vga_print_on_cursor(char* s) {
  uint_t pos = vga_get_cursor();

  int offs = pos << 1;
  char* mem = (char*)VGA_MEM_ADDR;

  for (char* p_ch = s; *p_ch != 0; p_ch++) {
    if (offs >= MODE_7_TEXT_OFFS_LIM * 2) {
      offs -= 160;
      vga_scroll();
    }

    if (*p_ch == '\n') {
      offs += 160 - (offs % 160);
      continue;
    } else if (*p_ch == '\t') {
      offs += 16 - (offs % 16);
      continue;
    }

    mem[offs] = *p_ch;
    mem[offs + 1] = TEXT_BLACK_ON_WHITE;

    offs += 2;
  }

  vga_set_cursor_to_offs(offs >> 1);
}

void vga_printl_on_cursor(char* s) {
  vga_print_on_cursor(s);
  vga_new_line();
}

void vga_new_line() {
  struct coord_t coord = vga_get_cursor_coord();
  vga_set_cursor(coord.y + 1, 0);
}

void vga_set_cursor_to_offs(uint_t offs) {
  byte_t pos_lo = offs & 0xff;
  byte_t pos_hi = (offs >> 8) & 0xff;

  io_byte_out(VGA_CTRL, CURSOR_POS_HI);
  io_byte_out(VGA_DATA, pos_hi);
  io_byte_out(VGA_CTRL, CURSOR_POS_LO);
  io_byte_out(VGA_DATA, pos_lo);
}

void vga_set_cursor(uint_t y, uint_t x) {
  uint_t offs = y * MODE_7_WIDTH + x;
  vga_set_cursor_to_offs(offs);
}

void vga_clear_screen() {
  int total = MODE_7_HEIGHT * MODE_7_WIDTH;
  char* mem = (char*)VGA_MEM_ADDR;

  mem_set(VGA_MEM_ADDR, total * 2, 0);
  // TODO: Fix missing cursor.

  vga_set_cursor(0, 0);
}

void vga_scroll() {
  for (int i = 1; i < MODE_7_HEIGHT; i++) {
    mem_copy(VGA_MEM_ADDR + i * 160, VGA_MEM_ADDR + (i - 1) * 160, 160);
  }
}
