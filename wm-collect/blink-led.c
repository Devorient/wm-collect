#include "sys/ctimer.h"
#include "blink-led.h"

static struct ctimer blink_timer;
static uint8_t set = 0;

static void
blink_callback(void *ptr) {
  rgb_led_off();
  set = 0;
}

void
blink_led(uint8_t color) {
  rgb_led_set(color);
  if (set) ctimer_restart(&blink_timer);
  else ctimer_set(&blink_timer, CLOCK_SECOND / 32, blink_callback, NULL);
  set = 1;
}