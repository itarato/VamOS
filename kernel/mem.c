#include "mem.h"

#include "util.h"
#include "vga.h"

/*

The current memory map:
- 0x1000 .. 0x7000 -> kernel temp load
- 0x7c00 .. 0x7dff -> boot loader
- 0x0010_0000 -> kernel copy destination + address
- 0x20_0000 .. 0x20_0FFF-> Page directory
- 0x20_1000 .. 0x20_1FFF-> 1st page table

*/

// https://wiki.osdev.org/Memory_Map_(x86)

// Set in boot.asm.
#define MEM_REGION_ADDR 0x8000
#define MEM_REGION_TYPE_USABLE 0x1

// The first "usable" region is from 0x0 - however
static u32 allocatable_mem_start = 0x202000;
static mem_region_desc_t mem_region_desc;

void setup_mem_regions() {
  mem_region_desc.len = *((u32*)MEM_REGION_ADDR);
  mem_region_desc.mem_regions = ((mem_region_info_t*)(MEM_REGION_ADDR + 4));

  for (int i = 0; i < mem_region_desc.len; i++) {
    printf("Mem region %x, start: %x len: %x type: %x\n", i,
           mem_region_desc.mem_regions[i].addr_lo,
           mem_region_desc.mem_regions[i].len_lo,
           mem_region_desc.mem_regions[i].type);
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

void* malloc(u32 size, u32 align) {
  u32 alloc_start = (allocatable_mem_start + align - 1) & ~(align - 1);

  for (int i = 0; i < mem_region_desc.len; i++) {
    if (mem_region_desc.mem_regions[i].type != MEM_REGION_TYPE_USABLE) continue;

    if (mem_region_desc.mem_regions[i].addr_hi > 0 ||
        mem_region_desc.mem_regions[i].len_hi > 0) {
      panic(">32bit address space is not yet supported");
      return NULL;
    }

    if (mem_region_desc.mem_regions[i].addr_lo > allocatable_mem_start) {
      alloc_start =
          (mem_region_desc.mem_regions[i].addr_lo + align - 1) & ~(align - 1);
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
