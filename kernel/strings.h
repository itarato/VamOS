#ifndef STRINGS_H
#define STRINGS_H

#include "types.h"

char* int_to_hex_string(u32 n, char* buf, u8 buflen);
bool is_regular_char(char c);
bool is_newline_char(char c);
bool is_backspace_char(char c);
bool is_tab_char(char c);
bool str_eq(char* lhs, char* rhs);
u32 strlen(char* s);

#endif
