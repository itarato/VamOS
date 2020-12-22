#ifndef DISK_H
#define DISK_H

#include "types.h"

void disk_init();
u8 disk_write(u8 drive, u32 lba, u8 num_sects, u16 selector, u32 edi);
u8 disk_read(u8 drive, u32 lba, u8 num_sects, u16 selector, u32 edi);

#endif
