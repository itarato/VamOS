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
    "<\\0>",       "<escape>", "1", "2", "3", "4",        "5",
    "6",           "7",        "8", "9", "0", "-",        "=",
    "<backspace>", "<tab>",    "q", "w", "e", "r",        "t",
    "y",           "u",        "i", "o", "p", "[",        "]",
    "<enter>",     "<lctrl>",  "a", "s", "d", "f",        "g",
    "h",           "j",        "k", "l", ";", "'",        "`",
    "<lshift>",    "\\",       "z", "x", "c", "v",        "b",
    "n",           "m",        ",", ".", "/", "<rshift>", "*",
    "<lalt>",      "<space>",
};

u8 scancode_ascii_map[256] = {
    0,   27, /* ESC */
    49,      /* 1 */
    50,      /* 2 */
    51,      /* 3 */
    52,      /* 4 */
    53,      /* 5 */
    54,      /* 6 */
    55,      /* 7 */
    56,      /* 8 */
    57,      /* 9 */
    48,      /* 0 */
    45,      /* - */
    61,      /* = */
    8,       /* BS */
    9,       /* HT */
    113,     /* q */
    119,     /* w */
    101,     /* e */
    114,     /* r */
    116,     /* t */
    121,     /* y */
    117,     /* u */
    105,     /* i */
    111,     /* o */
    112,     /* p */
    91,      /* [ */
    93,      /* ] */
    10,      /* LF */
    0,   97, /* a */
    115,     /* s */
    100,     /* d */
    102,     /* f */
    103,     /* g */
    104,     /* h */
    106,     /* j */
    107,     /* k */
    108,     /* l */
    59,      /* ; */
    39,      /* ' */
    96,      /* ` */
    0,   92, /* \ */
    122,     /* z */
    120,     /* x */
    99,      /* c */
    118,     /* v */
    98,      /* b */
    110,     /* n */
    109,     /* m */
    44,      /* , */
    46,      /* . */
    47,      /* / */
    0,   42, /* * */
    0,   32, /* space */
    0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0,   0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static void keyboard_callback(int_regs_t regs) {
  IGNORE(regs);

  // printl("key");
  u8 scancode = io_byte_in(KEYBOARD_DATA_PORT);

  u8 keymap_len = sizeof(keymap) / sizeof(char*);
  if (scancode >= keymap_len) {
    // char hexbuf[8];
    // print(int_to_hex_string((u32)scancode, hexbuf, 8));
    // vga_new_line();
    // printl("?");
  } else {
    // print(keymap[scancode]);

    for (u8 i = 0; i < LISTENERS_SIZE && listeners[i] != NULL; i++) {
      listeners[i](scancode_ascii_map[scancode]);
    }
  }
}

void keyboard_init() {
  register_irq_handler(1, keyboard_callback);

  for (u8 i = 0; i < LISTENERS_SIZE; i++)
    listeners[i] = (keyboard_listener_t)NULL;
}

void keyboard_subscribe(keyboard_listener_t listener) {
  u8 idx = 0;
  while (idx <= LISTENERS_SIZE && listeners[idx] != NULL) idx++;

  if (idx >= LISTENERS_SIZE) {
    panic("Cannot add more keyboard listener.");
    return;
  }

  listeners[idx] = listener;
}
