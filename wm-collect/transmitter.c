#include <stdio.h>
#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "sys/rtimer.h"
#include "blink-led.h"
#include "wm-collect.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define SEND_INTERVAL       (RTIMER_SECOND/2)

PROCESS(transmitter_process, "Transmitter process");
AUTOSTART_PROCESSES(&transmitter_process);

static packet_hdr_t tx_hdr;

static void
send_callback(struct rtimer *t, void *ptr) {
  rtimer_set(t, t->time + SEND_INTERVAL, 0, send_callback, ptr);
  
  tx_hdr.seq_num++;
  NETSTACK_NETWORK.output(NULL);
  LOG_DBG("Packet sent\n");
  blink_led(RGB_LED_BLUE);
}

PROCESS_THREAD(transmitter_process, ev, data)
{
  static struct rtimer send_task;

  PROCESS_BEGIN();

  LOG_INFO("Operating as the transmitter node\n");

  nullnet_buf = (uint8_t*)&tx_hdr;
  nullnet_len = PACKET_HEADER_LEN;

  tx_hdr.code = MESSAGE_CODE_LEGITIMATE;
  tx_hdr.seq_num = -1;
  tx_hdr.node_id = NODE_ID_TRANSMITTER;

  rtimer_set(&send_task, RTIMER_NOW() + SEND_INTERVAL, 0, send_callback, NULL);

  PROCESS_END();
}