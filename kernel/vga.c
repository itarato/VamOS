// TODO: Print format (numbers).
// TODO: Special chars.

#include "vga.h"

#include "io.h"
#include "mem.h"
#include "strings.h"

#define VGA_MEM_ADDR 0xb8000
#define VGA_CTRL 0x3d4
#define VGA_DATA 0x3d5
#define CURSOR_POS_HI 0xe
#define CURSOR_POS_LO 0xf

#define TEXT_BLACK_ON_WHITE 0xf

#define MODE_7_WIDTH 80
#define MODE_7_WIDTH_REAL 160
#define MODE_7_HEIGHT 25
#define MODE_7_TEXT_OFFS_LIM 2000

u32 vga_get_cursor() {
  io_byte_out(VGA_CTRL, CURSOR_POS_HI);
  u32 pos = io_byte_in(VGA_DATA);
  pos <<= 8;

  io_byte_out(VGA_CTRL, CURSOR_POS_LO);
  pos |= io_byte_in(VGA_DATA);

  return pos;
}

struct coord_t vga_get_cursor_coord() {
  u32 pos = vga_get_cursor();
  struct coord_t coord;

  coord.x = pos % MODE_7_WIDTH;
  coord.y = pos / MODE_7_WIDTH;

  return coord;
}

void printc(char c) {
  u32 pos = vga_get_cursor();

  int offs = pos << 1;
  char* mem = (char*)VGA_MEM_ADDR;

  if (offs >= MODE_7_TEXT_OFFS_LIM * 2) {
    offs -= MODE_7_WIDTH_REAL;
    vga_scroll();
  }

  if (is_newline_char(c)) {
    offs += MODE_7_WIDTH_REAL - (offs % MODE_7_WIDTH_REAL);
  } else if (is_tab_char(c)) {
    offs += 16 - (offs % 16);
  } else if (is_backspace_char(c)) {
    if (offs >= 2) {
      mem[offs - 2] = 0;
      mem[offs - 1] = TEXT_BLACK_ON_WHITE;

      offs -= 2;
    }
  } else {
    mem[offs] = c;
    mem[offs + 1] = TEXT_BLACK_ON_WHITE;

    offs += 2;
  }

  vga_set_cursor_to_offs(offs >> 1);
}

void print(char* s) {
  for (char* p_ch = s; *p_ch != 0; p_ch++) {
    printc(*p_ch);
  }
}

void printl(char* s) {
  print(s);
  vga_new_line();
}

void printhex(char* name, u32 v) {
  print(name);
  print(": ");

  char buf[9];
  int_to_hex_string(v, buf, 9);
  printl(buf);
}

void vga_new_line() {
  struct coord_t coord = vga_get_cursor_coord();
  vga_set_cursor(coord.y + 1, 0);
}

void vga_set_cursor_to_offs(u32 offs) {
  u8 pos_lo = offs & 0xff;
  u8 pos_hi = (offs >> 8) & 0xff;

  io_byte_out(VGA_CTRL, CURSOR_POS_HI);
  io_byte_out(VGA_DATA, pos_hi);
  io_byte_out(VGA_CTRL, CURSOR_POS_LO);
  io_byte_out(VGA_DATA, pos_lo);
}

void vga_set_cursor(u32 y, u32 x) {
  u32 offs = y * MODE_7_WIDTH + x;
  vga_set_cursor_to_offs(offs);
}

void vga_clear_screen() {
  u32 total = MODE_7_HEIGHT * MODE_7_WIDTH;
  char* mem = (char*)VGA_MEM_ADDR;

  for (u32 i = 0; i < total; i++) {
    mem[i * 2] = 0;
    mem[i * 2 + 1] = TEXT_BLACK_ON_WHITE;
  }

  vga_set_cursor(0, 0);
}

void vga_scroll() {
  char* mem = (char*)VGA_MEM_ADDR;
  for (u32 y = 1; y < MODE_7_HEIGHT; y++) {
    for (u32 x = 0; x < MODE_7_WIDTH * 2; x++) {
      mem[(y - 1) * (MODE_7_WIDTH * 2) + x] = mem[y * (MODE_7_WIDTH * 2) + x];
    }
  }

  for (u32 x = 0; x < MODE_7_WIDTH; x++) {
    mem[(MODE_7_HEIGHT - 1) * (MODE_7_WIDTH * 2) + (x * 2)] = 0;
    mem[(MODE_7_HEIGHT - 1) * (MODE_7_WIDTH * 2) + (x * 2) + 1] =
        TEXT_BLACK_ON_WHITE;
  }
}
