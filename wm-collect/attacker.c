#include <stdio.h>
#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "sys/rtimer.h"
#include "lib/random.h"
#include "sys/rtimer.h"
#include "sys/clock.h"
#include "blink-led.h"
#include "wm-collect.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

PROCESS(attacker_process, "Attacker process");
AUTOSTART_PROCESSES(&attacker_process);

static packet_hdr_t tx_hdr;
static packet_hdr_t *rx_hdr;

static void
recv_callback(const void *data, uint16_t len, const linkaddr_t *src, const linkaddr_t *dest) {
  if (len != PACKET_HEADER_LEN) return;

  rx_hdr = (packet_hdr_t*)data;

  if (rx_hdr->code != MESSAGE_CODE_LEGITIMATE || rx_hdr->node_id != NODE_ID_TRANSMITTER) return;
  LOG_DBG("Packet received\n");

  if (tx_hdr.node_id == 0) {
    // give the node an ID if it does not have one already
    tx_hdr.node_id = (random_rand() % (1000 - 150)) + 150;
    LOG_INFO("Node ID: %hu\n", tx_hdr.node_id);
  }

  clock_wait(5);

  tx_hdr.seq_num = rx_hdr->seq_num;
  NETSTACK_NETWORK.output(NULL);
  LOG_DBG("Packet sent\n");
  blink_led(RGB_LED_RED);
}

PROCESS_THREAD(attacker_process, ev, data)
{
  PROCESS_BEGIN();

  LOG_INFO("Operating as an attacker node\n");

  nullnet_buf = (uint8_t*)&tx_hdr;
  nullnet_len = PACKET_HEADER_LEN;
  nullnet_set_input_callback(recv_callback);

  tx_hdr.code = MESSAGE_CODE_ATTACK;
  tx_hdr.node_id = 0;

  PROCESS_END();
}