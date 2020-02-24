#include <stdio.h>
#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "sys/node-id.h"


#include "sys/log.h"
#define LOG_MODULE "App"
#define LOG_LEVEL LOG_LEVEL_INFO

#define LOG_REP(...)          LOG(1, LOG_LEVEL_INFO, "REP", __VA_ARGS__)
#define LOG_REP_(...)         LOG(0, LOG_LEVEL_INFO, "REP", __VA_ARGS__)

PROCESS(receiver_process, "Receiver process");
AUTOSTART_PROCESSES(&receiver_process);

static void
recv_callback(const void *data, uint16_t len, const linkaddr_t *src, const linkaddr_t *dest) {
  radio_value_t rssi;
  NETSTACK_RADIO.get_value(RADIO_PARAM_LAST_RSSI, &rssi);

  if (len != sizeof(uint32_t)) return;

  LOG_REP("%hu,%lu,%hhi\n", node_id, *(uint32_t*)data, rssi);
  
}

PROCESS_THREAD(receiver_process, ev, data)
{
  PROCESS_BEGIN();

  LOG_INFO("Operating as the receiver node\n");

  nullnet_set_input_callback(recv_callback);

  PROCESS_END();
}