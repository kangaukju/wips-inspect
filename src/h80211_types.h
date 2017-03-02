/*
 * h80211_types.h
 *
 *  Created on: 2016. 11. 29.
 *      Author: root
 */

#ifndef SRC_H80211_TYPES_H_
#define SRC_H80211_TYPES_H_

#include "ieee80211.h"

enum h28011_ds
{
	ds_adhoc  = 0x00,
	ds_tods   = 0x01,
	ds_fromds = 0x02,
	ds_wds    = 0x03,
};

enum h80211_frame
{
	frame_mgnt = 0x00,
	frame_ctrl = 0x01,
	frame_data = 0x02,
};

#define valid_h80211_frame(x) \
	(	((x) == frame_ctrl) || \
		((x) == frame_mgnt) || \
		((x) == frame_data)	)

enum h80211_mgnt
{
	mgnt_assoc_req   = 0x00,
	mgnt_assoc_resp  = 0x01,
	mgnt_reassoc_req = 0x02,
	mgnt_reassoc_resp= 0x03,
	mgnt_probe_req   = 0x04,
	mgnt_probe_resp  = 0x05,


	mgnt_beacon      = 0x08,
	mgnt_atim        = 0x09,
	mgnt_disassoc    = 0x0a,
	mgnt_auth        = 0x0b,
	mgnt_deauth      = 0x0c,
	mgnt_action      = 0x0d,
};

#define valid_h80211_mgnt(x) \
	(	(((x) >= mgnt_assoc_req) && ((x) <= mgnt_probe_resp)) || \
		(((x) >= mgnt_beacon)    && ((x) <= mgnt_action))	)

enum h80211_ctrl
{
//	ctrl_sector_sweep_feedback = 0x06,
//	ctrl_wrapper       = 0x07,
	ctrl_block_ack_req = 0x08,
	ctrl_block_ack     = 0x09,
	ctrl_ps_poll       = 0x0a,
	ctrl_rts           = 0x0b,
	ctrl_cts           = 0x0c,
	ctrl_ack           = 0x0d,
	ctrl_cf_end        = 0x0e,
	ctrl_cf_end_ack    = 0x0f,
};

#define valid_h80211_ctrl(x) \
	(	((x) >= ctrl_block_ack_req) && ((x) <= ctrl_cf_end_ack)	)

/**

type  | subtype
--------------------
b0 b1 | b2 b3 b4 b5
--------------------
 1  0 |  0  0  0  0   Data
 1  0 |  0  0  0  1   Data + CF-ACK  [PCF only]
 1  0 |  0  0  1  0   Data + CF-Poll [PCF only]
 1  0 |  0  0  1  1   Data + CF-ACK + CF-Poll [PCF only]
 1  0 |  0  1  0  0   Null (no data)
 1  0 |  0  1  0  1   CF-ACK (no data) [PCF only]
 1  0 |  0  1  1  0   CF-Poll (no data) [PCF only]
 1  0 |  0  1  1  1   CF-ACK + CF-Poll (no data) [PCF only]
 1  0 |  1  0  0  0   QoS Data [HCF]
 1  0 |  1  0  0  1   Qos Data + CF-ACK [HCF]
 1  0 |  1  0  1  0   Qos Data + CF-Poll [HCF]
 1  0 |  1  0  1  1   Qos Data + CF-ACK + CF-Poll [HCF]
 1  0 |  1  1  0  0   Qos Null (no data) [HCF]
 1  0 |  1  1  0  1   Reserved
 1  0 |  1  1  1  0   Qos CF-Poll (no data) [HCF]
 1  0 |  1  1  1  1   Qos CF-ACK + CF-Poll (no data) [HCF]

b5 = 1 (CF-ACK)
b4 = 1 (CF-Poll)
b3 = 1 (no data)
b2 = 1 (Qos data)

 */
enum h80211_data
{
	data_data              = 0x00,
	data_data_ack          = 0x01,
	data_data_poll         = 0x02,
	data_data_ack_poll     = 0x03,
	data_null              = 0x04,
	data_ack               = 0x05,
	data_poll              = 0x06,
	data_ack_poll          = 0x07,

	data_qos_data          = 0x08,
	data_qos_data_ack      = 0x09,
	data_qos_data_poll     = 0x0a,
	data_qos_data_ack_poll = 0x0b,
	data_qos_null          = 0x0c,
	data_reserved          = 0x0d,
	data_qos_poll          = 0x0e,
	data_qos_ack           = 0x0f,
};

#define valid_h80211_data(x) \
	(	((x) >= data_data) && ((x) <= data_qos_ack)	)


#endif /* SRC_H80211_TYPES_H_ */
