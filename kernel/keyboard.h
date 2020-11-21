#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "interrupt.h"

void keyboard_init();
static void keyboard_callback(int_regs_t regs);

#endif
