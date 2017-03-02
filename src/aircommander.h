/*
 * aircommander.h
 *
 *  Created on: 2017. 1. 16.
 *      Author: root
 */

#ifndef SRC_AIRCOMMANDER_H_
#define SRC_AIRCOMMANDER_H_

#include <stdint.h>

typedef struct req_hdr {
	uint32_t len;
	uint8_t  code;    // 1 bytes
	uint16_t type;    // 2 bytes
	uint8_t  subtype; // 1 bytes
} REQ_HDR;

typedef struct req {
	REQ_HDR  hdr;
	uint32_t datalen; // only internal used
	uint8_t  *data;
} REQ;


enum REQ_HDR_TYPE {
	REQ_HDR_TYPE_COMMAND = 0x01,
};

enum REQ_HDR_SUBTYPE {
	REQ_HDR_SUBTYPE_COMMAND_RET_RESULT      = 0x01,
	REQ_HDR_SUBTYPE_COMMAND_RET_IMMEDIATELY = 0x02,
};

enum REQ_HDR_CODE {
	REQ_HDR_CODE_REQ_REQUEST = 0x01,
	REQ_HDR_CODE_ACK_SUCCESS = 0x02,
	REQ_HDR_CODE_ACK_FAILURE = 0x04,
};

#endif /* SRC_AIRCOMMANDER_H_ */
