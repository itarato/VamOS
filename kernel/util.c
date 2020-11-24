#include "util.h"

#include "vga.h"

void panic(char *msg) {
  print("PANIC: ");
  printl(msg);
}
