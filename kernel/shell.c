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

void io_buf_pop() {
  u16 i;
  for (; char_buf[i] != 0; i++)
    ;

  if (i == 0) return;
  char_buf[i - 1] = 0;
}

void io_buf_clear() {
  for (u8 i = 0; i < CHAR_BUF_SIZE; i++) char_buf[i] = 0;
}

void shell_exec_buffer() {
  char* cmd = char_buf;

  vga_new_line();

  if (str_eq(cmd, "help")) {
    printl("VamOS Shell V0.0");
  } else if (str_eq(cmd, "exit")) {
    printl("Bye");
    halt();
  } else {
    printl("Unknown command");
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
  } else if (is_backspace_char(ch)) {
    io_buf_pop();
    printc(ch);
  }
}

void shell_init() {
  keyboard_subscribe(shell_keyboard_subscriber);
  io_buf_clear();
  print("#> ");
}
