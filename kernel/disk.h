#ifndef DISK_H
#define DISK_H

#include "types.h"

void disk_init();
void disk_write(u8 drive, u32 lba, u8 num_sects, u16 selector, u32 edi);
void disk_read(u8 drive, u32 lba, u8 num_sects, u16 selector, u32 edi);

#endif
