#ifndef TIMER_H
#define TIMER_H

#include "interrupt.h"
#include "types.h"

void timer_init(u32 freq);
u32 timer_tick_get();
u32 timer_tick_diff(u32);
void timer_sleep(u32);

#endif
