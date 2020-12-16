#include "mem.h"

#include "vga.h"

// https://wiki.osdev.org/Memory_Map_(x86)

#define MEM_REGION_ADDR 0x8000

static u32 allocatable_mem_start = 0x10000;
static mem_region_desc_t mem_region_desc;

void setup_mem_regions() {
  mem_region_desc.len = *((u32*)MEM_REGION_ADDR);
  mem_region_desc.mem_regions = ((mem_region_info_t*)(MEM_REGION_ADDR + 4));
}

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
