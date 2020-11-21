#include "mem.h"

void mem_copy(u32 src, u32 dest, u32 size) {
  char* p_src = (char*)src;
  char* p_dest = (char*)dest;

  for (u32 i = 0; i < size; i++) {
    p_dest[i] = p_src[i];
  }
}

void mem_set(u32 src, u32 size, unsigned char data) {
  char* p_src = (char*)src;

  for (u32 i = 0; i < size; i++) {
    p_src[i] = data;
  }
}
