/*
 * airresult.h
 *
 *  Created on: 2017. 1. 13.
 *      Author: root
 */

#ifndef SRC_AIRRESULT_H_
#define SRC_AIRRESULT_H_

#include "airversion.h"
#include "airconf-parse.h"


enum {
	AIRRESULT_CAPTURE_PORT = 8554,
	AIRRESULT_CURRENT_PORT = 8555,
};

struct ar_avp {
	uint32_t id;
	uint8_t type;
	uint32_t length;
	uint8_t* value;
	struct ar_avp* next;
};

struct ar_msg {
	uint8_t type;
	uint8_t version;
	uint8_t flags;
	uint32_t length;
	struct ar_avp* avp;
};

enum AR_MSG_TYPE {
	AR_MSG_CAPTURE_RESULT = 1,
};

enum AR_AVP_TYPE {
	AR_AVP_TYPE_INT8 = 1,
	AR_AVP_TYPE_INT16,
	AR_AVP_TYPE_INT32,
	AR_AVP_TYPE_INT64,

	AR_AVP_TYPE_UINT8,
	AR_AVP_TYPE_UINT16,
	AR_AVP_TYPE_UINT32,
	AR_AVP_TYPE_UINT64,

	AR_AVP_TYPE_LONG,

	AR_AVP_TYPE_STRING,
	AR_AVP_TYPE_BIN,
};

enum AR_AVP_ID {
	AR_AVP_ID_PROFILE_ID = 1,
	AR_AVP_ID_CONFIG_ID,
	AR_AVP_ID_KEY,
	AR_AVP_ID_XID,
	AR_AVP_ID_SEC,
	AR_AVP_ID_USEC,
	AR_AVP_ID_ELAPSED,
	AR_AVP_ID_PWR,
	AR_AVP_ID_FRAME_TYPE,
	AR_AVP_ID_FRAME_SUBTYPE,
	AR_AVP_ID_ADDR1,
	AR_AVP_ID_ADDR2,
	AR_AVP_ID_ADDR3,
	AR_AVP_ID_ADDR4,
	AR_AVP_ID_DS,
	AR_AVP_ID_SEQ,
	AR_AVP_ID_DEAUTH_REASON,
};
uint8_t get_airresult_version();

#endif /* SRC_AIRRESULT_H_ */
