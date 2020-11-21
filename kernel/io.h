#ifndef IO_H
#define IO_H

#include "types.h"

byte_t io_byte_in(word_t port);
void io_byte_out(word_t port, byte_t data);

word_t io_word_in(word_t port);
void io_word_out(word_t port, word_t data);

#endif
