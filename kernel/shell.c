#include "shell.h"

#include "keyboard.h"

static void shell_keyboard_subscriber(u8 ch) {}

void shell_init() { keyboard_subscribe(shell_keyboard_subscriber); }
