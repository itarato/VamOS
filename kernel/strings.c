#include "strings.h"

#include "types.h"

bool is_regular_char(char c) {
  return (c >= 97 && c <= 122) || (c >= 48 && c <= 57);
}
