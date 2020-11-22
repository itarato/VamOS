#include "keyboard.h"

#include "interrupt.h"
#include "io.h"
#include "types.h"
#include "util.h"
#include "vga.h"

#define KEYBOARD_DATA_PORT 0x60

#define LISTENERS_SIZE 4
static keyboard_listener_t listeners[LISTENERS_SIZE];

// https://wiki.osdev.org/PS2_Keyboard
char* keymap[] = {
    "<\\0>",       "<escape>", "1", "2", "3",  "4",        "5",
    "6",           "7",        "8", "9", "0",  "-",        "=",
    "<backspace>", "<tab>",    "Q", "W", "E",  "R",        "T",
    "Y",           "U",        "I", "O", "P",  "[",        "]",
    "<enter>",     "<lctrl>",  "A", "S", "D",  "F",        "G",
    "H",           "J",        "K", "L", " ;", "'",        "`",
    "<lshift>",    "\\",       "Z", "X", "C",  "V",        "B",
    "N",           "M",        ",", ".", "/",  "<rshift>", "*",
    "<lalt>",      "<space>",
};

static void keyboard_callback(int_regs_t regs) {
  // vga_printl_on_cursor("key");
  u8 scancode = io_byte_in(KEYBOARD_DATA_PORT);

  u8 keymap_len = sizeof(keymap) / sizeof(char*);
  if (scancode >= keymap_len) {
    // vga_print_hex((u32)scancode);
    // vga_new_line();
    vga_printl_on_cursor("?");
  } else {
    vga_printl_on_cursor(keymap[scancode]);
  }
}

void keyboard_init() {
  register_irq_handler(1, keyboard_callback);

  for (u8 i = 0; i < LISTENERS_SIZE; i++)
    listeners[i] = (keyboard_listener_t)NULL;
}

void keyboard_subscribe(keyboard_listener_t listener) {
  u8 idx = 0;
  while (idx <= LISTENERS_SIZE && listeners[idx++] == NULL)
    ;

  if (idx >= LISTENERS_SIZE) {
    panic("Cannot add more keyboard listener.");
    return;
  }

  idx--;
  listeners[idx] = listener;
}
