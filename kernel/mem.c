#include "mem.h"

void mem_copy(uint_t src, uint_t dest, uint_t size) {
  char* p_src = (char*)src;
  char* p_dest = (char*)dest;

  for (uint_t i = 0; i < size; i++) {
    p_dest[i] = p_src[i];
  }
}

void mem_set(uint_t src, uint_t size, unsigned char data) {
  char* p_src = (char*)src;

  for (uint_t i = 0; i < size; i++) {
    p_src[i] = data;
  }
}
