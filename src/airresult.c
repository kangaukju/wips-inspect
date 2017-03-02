/*
 * airresult.c
 *
 *  Created on: 2017. 1. 13.
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>

#include "airresult.h"


uint8_t get_airresult_version() {
	return AIRVERSION;
}

struct ar_avp* ar_avp_new(struct ar_avp *prev, uint32_t id, uint8_t type, uint32_t length, uint8_t* value)
{
	struct ar_avp *avp = (struct ar_avp *)malloc(sizeof(struct ar_avp));

	avp->id = id;
	avp->type = type;
	avp->length = length;
	if (length == 0) {
		avp->value = NULL;
	} else {
		avp->value = (uint8_t*)malloc(length);
		memcpy(avp->value, value, length);
	}
	if (prev) {
		prev->next = avp;
	}
	avp->next = NULL;
	return avp;
}

int ar_avp_free(struct ar_avp *first)
{
	struct ar_avp *cur = NULL;
	struct ar_avp *next = NULL;
	int count = 0;

	if (!first) {
		return 0;
	}

	cur = first;
	while (cur) {
		next = cur->next;

		if (cur->value) {
			free(cur->value);
			cur->value = NULL;
		}
		free(cur);

		cur = next;

		count++;
	}
	return count;
}

uint32_t ar_avp_total_length(struct ar_avp *first)
{
	struct ar_avp *cur = NULL;
	uint32_t total = 0;

	if (!first) {
		return 0;
	}
	for (cur = first; cur; cur = cur->next) {
		total += cur->length;
	}
	return total;
}
