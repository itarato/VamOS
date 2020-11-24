#ifndef STRINGS_H
#define STRINGS_H

#include "types.h"

char* int_to_hex_string(u32 n, char* buf, u8 buflen);
bool is_regular_char(char c);

#endif
