#include <stdio.h>
#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "sys/clock.h"
#include "blink-led.h"
#include "wm-collect.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

PROCESS(receiver_process, "Receiver process");
AUTOSTART_PROCESSES(&receiver_process);

static uint8_t tx_buffer[128];
static packet_hdr_t *tx_hdr = (packet_hdr_t*)tx_buffer;
static packet_report_t *tx_report = (packet_report_t*)&tx_buffer[PACKET_CONTENT_START];

static packet_hdr_t *rx_hdr;

static void
recv_callback(const void *data, uint16_t len, const linkaddr_t *src, const linkaddr_t *dest) {
  radio_value_t rssi, lqi;
  NETSTACK_RADIO.get_value(RADIO_PARAM_LAST_RSSI, &rssi);
  NETSTACK_RADIO.get_value(RADIO_PARAM_LAST_LINK_QUALITY, &lqi);

  if (len != PACKET_HEADER_LEN) return;

  rx_hdr = (packet_hdr_t*)data;
  LOG_INFO("Packet received from %d\n", rx_hdr->node_id);
  if (rx_hdr->code == MESSAGE_CODE_LEGITIMATE) blink_led(RGB_LED_BLUE);
  else if (rx_hdr->code == MESSAGE_CODE_ATTACK) blink_led(RGB_LED_RED);
  else return;

  tx_report->sender_node_id = rx_hdr->node_id;
  tx_report->seq_num = rx_hdr->seq_num;
  tx_report->time_stamp = clock_time();
  tx_report->rssi = rssi;
  tx_report->lqi = lqi;
  
  NETSTACK_NETWORK.output(NULL);
  LOG_DBG("Packet sent\n");
}

PROCESS_THREAD(receiver_process, ev, data)
{
  PROCESS_BEGIN();

  LOG_INFO("Operating as the receiver node\n");

  nullnet_buf = tx_buffer;
  nullnet_len = PACKET_REPORT_LEN;
  nullnet_set_input_callback(recv_callback);

  tx_hdr->code = MESSAGE_CODE_REPORT;
  tx_hdr->node_id = NODE_ID_RECEIVER;

  PROCESS_END();
}