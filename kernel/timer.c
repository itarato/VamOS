#include "timer.h"

#include "interrupt.h"
#include "io.h"
#include "types.h"
#include "util.h"
#include "vga.h"

#define MAIN_FREQ 1193180

#define TIMER_PORT_CH0 0x40
#define TIMER_PORT_CH1 0x41
#define TIMER_PORT_CH2 0x42
#define TIMER_MODE_CMD 0x43

static u32 tick;

static void timer_callback(int_regs_t* regs) {
  IGNORE(regs);
  tick++;
}

u32 timer_tick_get() { return tick; }

u32 timer_tick_diff(u32 start_tick) {
  if (tick >= start_tick) {
    return tick - start_tick;
  } else {
    return (u32)~start_tick + tick;
  }
}

void timer_sleep(u32 ticks) {
  u32 start = timer_tick_get();
  while (timer_tick_diff(start) < ticks)
    ;
}

void timer_init(u32 freq) {
  u32 div = MAIN_FREQ / freq;
  u8 lo = (u8)(div & 0xff);
  u8 hi = (u8)((div >> 8) & 0xff);

  io_byte_out(TIMER_MODE_CMD, 0x36);
  io_byte_out(TIMER_PORT_CH0, lo);
  io_byte_out(TIMER_PORT_CH0, hi);

  register_irq_handler(0, timer_callback);
}
