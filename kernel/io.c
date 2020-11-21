#include "io.h"

u8 io_byte_in(u16 port) {
  u8 res;
  __asm__("in %%dx, %%al" : "=a"(res) : "d"(port));
  return res;
}

void io_byte_out(u16 port, u8 data) {
  __asm__("out %%al, %%dx" : : "a"(data), "d"(port));
}

u16 io_word_in(u16 port) {
  u16 res;
  __asm__("in %%dx, %%ax" : "=a"(res) : "d"(port));
  return res;
}

void io_word_out(u16 port, u16 data) {
  __asm__("out %%ax, %%dx" : : "a"(data), "d"(port));
}
