#ifndef SHELL_H
#define SHELL_H

#include "types.h"

// TODO: This might not be the best place for this - more like an IO but maybe
// not the IO we have currently.
#define CHAR_BUF_SIZE 255
static u8 char_buf[CHAR_BUF_SIZE];

void shell_init();

#endif
