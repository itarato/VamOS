#include "mem.h"

// https://wiki.osdev.org/Memory_Map_(x86)

static u32 allocatable_mem_start = 0x10000;

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

void* malloc(u32 size) {
  void* p = (void*)allocatable_mem_start;

  allocatable_mem_start += size;

  return p;
}

// u16 get_available_low_memory() {
//   u16 size;
//   // asm("movw ($mem_size), %%ax" : "=a"(size));
//   asm()
//   return size;
// }
