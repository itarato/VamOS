#ifndef IO_H
#define IO_H

#include "types.h"

u8 io_byte_in(u16 port);
void io_byte_out(u16 port, u8 data);

u16 io_word_in(u16 port);
void io_word_out(u16 port, u16 data);

u32 io_dword_in(u16 port);

void io_dword_seq_in(u16 port, void* buf, u32 len);

#endif
