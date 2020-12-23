#include "strings.h"

#include "types.h"
#include "util.h"

char* int_to_hex_string(u32 n, char* buf, u8 buflen) {
  u32 len = 0;
  u32 _n = n;
  for (; _n > 0; len++) _n /= 16;

  if (buflen <= len + 2) panic("Buffer too small");

  for (int i = 0; i < buflen; i++) buf[i] = 0;

  buf[0] = '0';
  buf[1] = 'x';

  u32 i = 2;
  for (; n > 0; n >>= 4) {
    int hexdigit = n & 0xf;
    buf[3 + len - i] = hexdigit <= 9 ? '0' + hexdigit : 'a' + (hexdigit - 10);
    i++;
  }

  if (i == 2) buf[2] = '0';

  return buf;
}

bool is_regular_char(char c) {
  return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == ' ';
}

bool is_newline_char(char c) { return c == '\n'; }

bool is_tab_char(char c) { return c == '\t'; }

bool is_backspace_char(char c) { return (u8)c == 8; }

bool str_eq(char* lhs, char* rhs) {
  while (*lhs != 0 && *lhs == *rhs) {
    lhs++;
    rhs++;
  }

  return *lhs == 0 && *rhs == 0;
}

u32 strlen(char* s) {
  u32 len = 0;
  for (; *(s + len) != 0; len++)
    ;
  return len;
}