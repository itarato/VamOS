#include "shell.h"

#include "keyboard.h"
#include "vga.h"

static void shell_keyboard_subscriber(u8 ch) {
  char s[2] = {(char)ch, (char)0};
  vga_print_on_cursor(s);
}

void shell_init() { keyboard_subscribe(shell_keyboard_subscriber); }
