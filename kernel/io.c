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

u32 io_dword_in(u16 port) {
  u32 res;
  asm("in %%edx, %%eax" : "=a"(res) : "d"(port));
  return res;
}

void io_dword_seq_in(u16 port, void* buf, u32 len) {
  while (len--) {
    *(u32*)buf = io_dword_in(port);
    buf += 4;
  }
}
