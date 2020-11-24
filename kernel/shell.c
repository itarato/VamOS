#include "shell.h"

#include "keyboard.h"
#include "strings.h"
#include "types.h"
#include "util.h"
#include "vga.h"

static char char_buf[CHAR_BUF_SIZE];

void io_buf_push(char ch) {
  u16 i;
  for (; char_buf[i] != 0; i++)
    ;

  if (i >= CHAR_BUF_SIZE - 1) panic("Char buf overflow.");

  char_buf[i] = ch;
}

void io_buf_clear() {
  for (u8 i = 0; i < CHAR_BUF_SIZE; i++) char_buf[i] = 0;
}

void shell_exec_buffer() {
  char* cmd = char_buf;

  if (str_eq(cmd, "help")) {
    printl("\n VamOS Shell V0.0");
  } else {
    printl("\n Unknown command");
  }

  io_buf_clear();
}

static void shell_keyboard_subscriber(u8 ch) {
  if (is_regular_char(ch)) {
    printc(ch);
    io_buf_push(ch);
  } else if (is_newline_char(ch)) {
    shell_exec_buffer();

    print("\n#> ");
  }
}

void shell_init() {
  keyboard_subscribe(shell_keyboard_subscriber);
  io_buf_clear();
  print("#> ");
}
