#ifndef MEM_H
#define MEM_H

#include "types.h"

typedef struct {
  u32 addr_lo;
  u32 addr_hi;
  u32 len_lo;
  u32 len_hi;
  u32 type;
  u32 __unused;
} __attribute__((packed)) mem_region_info_t;

typedef struct {
  u32 len;
  mem_region_info_t* mem_regions;
} mem_region_desc_t;

void setup_mem_regions();
void mem_copy(u32 src, u32 dest, u32 size);
void mem_set(u32 src, u32 size, unsigned char data);
// @param align - must be power of 2.
void* malloc(u32 size, u32 align);

#endif
