#ifndef WM_COLLECT_H_
#define WM_COLLECT_H_
#include <stdint.h>

typedef enum message_code {
  MESSAGE_CODE_LEGITIMATE = 100,
  MESSAGE_CODE_ATTACK,
  MESSAGE_CODE_REPORT
} message_code_t;

typedef enum node_id {
  NODE_ID_TRANSMITTER = 100,
  NODE_ID_RECEIVER,
} node_id_t;

struct packet_hdr {
  uint32_t seq_num;
  message_code_t code;
  uint16_t node_id;
};

struct packet_report {
  uint16_t sender_node_id;
  clock_time_t time_stamp;
  uint32_t seq_num;
  int8_t rssi;
  uint8_t lqi;
};

typedef struct packet_hdr packet_hdr_t;
typedef struct packet_report packet_report_t;

#define PACKET_HEADER_LEN       (sizeof(packet_hdr_t))
#define PACKET_CONTENT_START    PACKET_HEADER_LEN
#define PACKET_REPORT_LEN       (PACKET_HEADER_LEN + sizeof(packet_report_t))

#endif