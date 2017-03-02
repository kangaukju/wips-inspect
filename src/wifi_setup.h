/*
 * wifi_setup.h
 *
 *  Created on: 2016. 12. 5.
 *      Author: root
 */

#ifndef SRC_WIFI_SETUP_H_
#define SRC_WIFI_SETUP_H_

#include "osdep/osdep.h"
#include "osdep/common.h"

#define MAX_WIFI_CARDS 8 /* maximum number of cards to capture from */

void close_wifi(struct wif *wi);
int check_wi_dev(struct wif *wi, uint8_t channel);
int init_wifi_cards(const char* cardnames, char *ifaces[], struct wif *wi[]);
int setup_wifi_card(char *iface, struct wif **wi);

#endif /* SRC_WIFI_SETUP_H_ */
