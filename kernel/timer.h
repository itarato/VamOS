#ifndef TIMER_H
#define TIMER_H

#include "interrupt.h"

void timer_init(u32 freq);
static void timer_callback(int_regs_t regs);

#endif
