/*
 * wi_packet.h
 *
 *  Created on: 2016. 12. 5.
 *      Author: root
 */

#ifndef SRC_WI_PACKET_H_
#define SRC_WI_PACKET_H_

#include "common.h"
#include "osdep/packed.h"

#define MAX_IE_ELEMENT_SIZE 256


typedef struct _wi_flags {
	uint8_t   tods:1;
	uint8_t   fromds:1;
	uint8_t   more_frag:1;
	uint8_t   retry:1;
	uint8_t   pwr_mgt:1;
	uint8_t   more_data:1;
	uint8_t   protect:1;
	uint8_t   order_flag:1;
} __packed
wi_flags; // 1 byte

typedef struct _wi_fc_t {
	uint8_t  version:2;
	uint8_t  type:2;
	uint8_t  subtype:4;
	wi_flags flags; // 1 byte
} __packed
wi_fc_t; // 2 byte

typedef struct _wi_fragseq {
	uint16_t    frag_number:4;
	uint16_t    seq_number:12;
} __packed
wi_fragseq; // 2 byte

typedef struct _wi_hdr {
	wi_fc_t    fc;        // 2 byte
	uint16_t   duration;  // 2 byte
	uint8_t    addr1[6];  // 6 byte
	uint8_t    addr2[6];  // 6 byte
	uint8_t    addr3[6];  // 6 byte
	wi_fragseq frag_seq;  // 2 byte
	uint8_t    addr4[0];  // 0 byte (6 byte)
} __packed
wi_hdr; // 24 byte (30 byte)

typedef struct _wi_ctrl_hdr {
	wi_fc_t  fc;       // 2 byte
	uint16_t duration; // 2 byte
	uint8_t  addr1[6]; // 6 byte
	uint8_t  addr2[0]; // 0 byte (6 byte)
} __packed
wi_ctrl_hdr; // 10 byte (16 byte)

typedef struct _wi_tag {
	uint8_t   tag;
	uint8_t   len;
	uint8_t   data[0];
} __packed
wi_tag;

typedef struct _wi_beacon_capa {
	uint16_t   ess_capa:1;
	uint16_t   ibss:1;
	uint16_t   cfp_1:2;
	uint16_t   privacy:1;
	uint16_t   preamble:1;
	uint16_t   pbcc:1;
	uint16_t   channel_agility:1;
	uint16_t   spec_mgnt:1;
	uint16_t   cfp_2:1;
	uint16_t   short_slot_time:1;
	uint16_t   auto_power_save:1;
	uint16_t   radio_mesurment:1;
	uint16_t   dsss_ofdm:1;
	uint16_t   block_ack:1;
	uint16_t   imm_block_ack:1;
} __packed
wi_bcn_capa; // 2 byte

typedef struct _wi_beacon_fixed {
	unsigned long long timestamp; // 8 byte
	uint16_t           intv;      // 2 byte (beacon interval)
	wi_bcn_capa        capa;      // 2 byte
} __packed
wi_bcn_fixed; // 12 byte

typedef struct _wi_assoc_req_fixed {
	wi_bcn_capa capa;        // 2 byte
	uint16_t    listen_intv; // 2 byte
} __packed
wi_assoc_req_fixed;   // 4 byte

typedef struct _wi_reassoc_req_fixed {
	wi_bcn_capa capa;        // 2 byte
	uint16_t    listen_intv; // 2 byte
	uint8_t     cur_ap[6];   // 6 byte
} __packed
wi_reassoc_req_fixed; // 10 byte

typedef struct _wi_assoc_resp_fixed {
	wi_bcn_capa capa;       // 2 byte
	uint16_t    status;     // 2 byte, 0x0000 = successful
	uint16_t    assoc_id;   // 2 byte
} __packed
wi_assoc_resp_fixed; // 6 byte

typedef wi_assoc_resp_fixed
wi_reassoc_resp_fixed; // 6 byte

typedef struct _wi_auth_fixed {
	uint16_t   algorithm; // 2 byte (0: open system, 1: shared key)
	uint16_t   seq;       // 2 byte
	uint16_t   status;    // 2 byte (0x0000: successful)
} __packed
wi_auth_fixed; // 6 byte

typedef struct _wi_deauth_fixed {
	uint16_t   reason; // 2 byte
} __packed
wi_deauth_fixed; // 2 byte

typedef struct _wi_ctrl_ba_control {
	uint16_t   immediate_ack:1;
	uint16_t   multi_tid:1;
	uint16_t   compress_bitmap:1;
	uint16_t   reserved:9;
	uint16_t   tid_for_frame:4;
} __packed
wi_ctrl_ba_ctrl; // 2 byte

typedef struct _wi_ctrl_ba_starting_sequence_control {
	uint16_t   fragment:4;
	uint16_t   starting_seq:12;
} __packed
wi_ctrl_ba_ssc; // 2 byte

typedef struct _wi_ctrl_ba_bitmap {
	uint8_t    bitmap[8];
} __packed
wi_ctrl_ba_bitmap; // 8 byte

typedef struct _wi_qos_control {
	uint16_t   priority:3;
	uint16_t   not_use:1; // not use
	uint16_t   eosp:1;
	uint16_t   ack_policy:2;
	uint16_t   payload_type:1;
	uint16_t   qap_ps_buffer_state:8;
} __packed
wi_qos_control; // 2 byte

typedef struct _wi_data_llc {
	uint8_t dsap;
	uint8_t ssap;
	uint8_t ctrl;
	uint8_t oui[3];
	uint8_t pid[2];
} __packed
wi_data_llc; // 8 byte

enum beacon_ie
{
	beacon_ie_ssid         = 0,
	beacon_ie_support_rate = 1,
};


#endif /* SRC_WI_PACKET_H_ */
