#include "shell.h"

#include "keyboard.h"
#include "strings.h"
#include "types.h"
#include "vga.h"

static void shell_keyboard_subscriber(u8 ch) {
  char s[2] = {(char)ch, (char)0};
  print(s);

  if (is_regular_char((char)ch)) {
  } else {
  }
}

void shell_init() { keyboard_subscribe(shell_keyboard_subscriber); }
