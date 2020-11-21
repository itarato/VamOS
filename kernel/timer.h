#ifndef TIMER_H
#define TIMER_H

#include "interrupt.h"

void timer_init(unsigned int freq);
static void timer_callback(isr_call_stack_t regs);

#endif
