#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "interrupt.h"

void keyboard_init();

typedef void (*keyboard_listener_t)(u8);
void keyboard_subscribe(keyboard_listener_t listener);

#endif
