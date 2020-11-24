#include "strings.h"

#include "types.h"
#include "util.h"

char* int_to_hex_string(u32 n, char* buf, u8 buflen) {
  u32 len = 0;
  for (; (n >> (len * 4)) > 0; len++)
    ;

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
  return (c >= 97 && c <= 122) || (c >= 48 && c <= 57);
}
