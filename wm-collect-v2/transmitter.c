#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "sys/rtimer.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define SEND_INTERVAL       (RTIMER_SECOND/200)

PROCESS(transmitter_process, "Transmitter process");
AUTOSTART_PROCESSES(&transmitter_process);

static void
send_callback(struct rtimer *t, void *ptr) {
  rtimer_set(t, t->time + SEND_INTERVAL, 0, send_callback, ptr);

  (*(uint32_t*)ptr)++;

  NETSTACK_NETWORK.output(NULL);
  LOG_DBG("Packet sent. seq_num = %lu\n", *(uint32_t*)ptr - 1);
}

PROCESS_THREAD(transmitter_process, ev, data)
{
  static struct rtimer send_task;
  static uint32_t seq_num;

  PROCESS_BEGIN();

  LOG_INFO("Operating as the transmitter node\n");

  nullnet_buf = (uint8_t*)&seq_num;
  nullnet_len = sizeof(uint32_t);

  rtimer_set(&send_task, RTIMER_NOW() + SEND_INTERVAL, 0, send_callback, (void*)&seq_num);

  PROCESS_END();
}