#include "mem.h"

#include "util.h"
#include "vga.h"

// https://wiki.osdev.org/Memory_Map_(x86)

#define MEM_REGION_ADDR 0x8000
#define MEM_REGION_TYPE_USABLE 0x2

static u32 allocatable_mem_start = 0x0;
static mem_region_desc_t mem_region_desc;

void setup_mem_regions() {
  mem_region_desc.len = *((u32*)MEM_REGION_ADDR);
  mem_region_desc.mem_regions = ((mem_region_info_t*)(MEM_REGION_ADDR + 4));

  for (int i = 0; i < mem_region_desc.len; i++) {
    printhex("Mem region", i);
    printhex("  - start", mem_region_desc.mem_regions[i].addr_lo);
    printhex("  - len", mem_region_desc.mem_regions[i].len_lo);
    // printhex("  - available", mem_region_desc.mem_regions[i].type);
  }
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
  u32 alloc_start = allocatable_mem_start;

  for (int i = 0; i < mem_region_desc.len; i++) {
    if (mem_region_desc.mem_regions[i].type != MEM_REGION_TYPE_USABLE) continue;

    if (mem_region_desc.mem_regions[i].addr_hi > 0 ||
        mem_region_desc.mem_regions[i].len_hi > 0) {
      panic(">32bit address space is not yet supported");
      return NULL;
    }

    if (mem_region_desc.mem_regions[i].addr_lo > allocatable_mem_start) {
      alloc_start = mem_region_desc.mem_regions[i].addr_lo;
    }

    if (mem_region_desc.mem_regions[i].addr_lo +
            mem_region_desc.mem_regions[i].len_lo <
        alloc_start + size) {
      continue;
    }

    allocatable_mem_start = alloc_start + size;
    return (void*)alloc_start;
  }

  return NULL;
}
