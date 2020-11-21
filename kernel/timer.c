#include "timer.h"

#include "interrupt.h"
#include "io.h"
#include "vga.h"

#define MAIN_FREQ 1193180

#define TIMER_PORT_CH0 0x40
#define TIMER_PORT_CH1 0x41
#define TIMER_PORT_CH2 0x42
#define TIMER_MODE_CMD 0x43

void timer_init(unsigned int freq) {
  unsigned int div = MAIN_FREQ / freq;
  byte_t lo = (byte_t)(div & 0xff);
  byte_t hi = (byte_t)((div >> 8) & 0xff);

  io_byte_out(TIMER_MODE_CMD, 0x36);
  io_byte_out(TIMER_PORT_CH0, lo);
  io_byte_out(TIMER_PORT_CH0, hi);

  register_irq_handler(0, timer_callback);
}

static void timer_callback(isr_call_stack_t regs) {
  vga_printl_on_cursor("tick");
}
