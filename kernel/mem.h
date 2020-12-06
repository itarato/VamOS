#ifndef MEM_H
#define MEM_H

#include "types.h"

void mem_copy(u32 src, u32 dest, u32 size);
void mem_set(u32 src, u32 size, unsigned char data);
void* malloc(u32 size);

#endif
