#include <stdio.h>
#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "blink-led.h"
#include "wm-collect.h"

#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

PROCESS(wm_collect, "WM collect");
AUTOSTART_PROCESSES(&wm_collect);

static packet_hdr_t *rx_hdr;
static packet_report_t *rx_report;

static void
report_print(packet_report_t *report) {
  // sender_node_id, seq_num, time_stamp, rssi, lqi
  printf("[REP] %hu,%lu,", 
         report->sender_node_id, report->seq_num);
  printf("%lu,%hhi,%hhu\n", report->time_stamp,
         report->rssi, report->lqi);
  
}

static void
report_recv(const void *data, uint16_t len, const linkaddr_t *src, const linkaddr_t *dest) {
  if (len != PACKET_REPORT_LEN) return;
  rx_hdr = (packet_hdr_t*)data;
  rx_report = (packet_report_t*)&((uint8_t*)data)[PACKET_CONTENT_START];

  if (rx_hdr->code != MESSAGE_CODE_REPORT) return;
  LOG_DBG("Report received from %d\n", rx_hdr->node_id);
  blink_led(RGB_LED_GREEN);

  report_print(rx_report);
}



PROCESS_THREAD(wm_collect, ev, data)
{
  PROCESS_BEGIN();

  LOG_INFO("Operating as the reporter node\n");

  nullnet_set_input_callback(report_recv);

  PROCESS_END();
}