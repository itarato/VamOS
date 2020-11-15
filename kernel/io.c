#include "io.h"

byte_t io_byte_in(word_t port) {
  byte_t res;
  __asm__("in %%dx, %%al" : "=a"(res) : "d"(port));
  return res;
}

void io_byte_out(word_t port, byte_t data) {
  __asm__("out %%al, %%dx" : : "a"(data), "d"(port));
}

word_t io_word_in(word_t port) {
  word_t res;
  __asm__("in %%dx, %%ax" : "=a"(res) : "d"(port));
  return res;
}

void io_word_out(word_t port, word_t data) {
  __asm__("out %%ax, %%dx" : : "a"(data), "d"(port));
}
