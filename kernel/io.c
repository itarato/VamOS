#include "io.h"

u8 io_byte_in(u16 port) {
  u8 res;
  asm("in %%dx, %%al" : "=a"(res) : "d"(port));
  return res;
}

void io_byte_out(u16 port, u8 data) {
  asm("out %%al, %%dx" : : "a"(data), "d"(port));
}

u16 io_word_in(u16 port) {
  u16 res;
  asm("in %%dx, %%ax" : "=a"(res) : "d"(port));
  return res;
}

void io_word_out(u16 port, u16 data) {
  asm("out %%ax, %%dx" : : "a"(data), "d"(port));
}
