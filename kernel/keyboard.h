#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "interrupt.h"

void keyboard_init();
static void keyboard_callback(isr_call_stack_t regs);

#endif
