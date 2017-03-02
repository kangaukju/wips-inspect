/*
 * wifi_setup.c
 *
 *  Created on: 2016. 12. 5.
 *      Author: root
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <termios.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <assert.h>

#include "wifi_setup.h"
#include "common.h"

void close_wifi(struct wif *wi)
{
	if (!wi) return;

	char *path = linux_path("wi_close");
	if (path) {
		int cmdlen = strlen(path) + MAX_IFACE_NAME + 2;
		char *cmd = (char*)calloc(1, cmdlen);
		snprintf(cmd, cmdlen, "%s %s", path, wi_get_ifname(wi));
		int ret = system(cmd);
		free(cmd);
		free(path);
	} else {
		wi_close(wi);
	}
}

int check_wi_dev(struct wif *wi, uint8_t channel)
{
	if (wi == NULL)
		return -1;

	if (channel > 0)
	{
		if (wi_set_channel(wi, channel))
			return -1;
	}
	return 0;
}

int setup_wifi_card(char *iface, struct wif **wi)
{
	struct wif *_wi = NULL;

	_wi = wi_open(iface);
	if (!wi)
		return -1;

	*wi = _wi;
	return 0;
}

int init_wifi_cards(const char* cardnames, char *ifaces[], struct wif *wi[])
{
	char *buffer = NULL;
	char *buf = NULL;
	int if_count = 0;
	int i = 0;
	int dup = 0;

	buffer = buf = (char *) malloc(sizeof(char) * 1025);
	strncpy(buffer, cardnames, 1025);
	buffer[1024] = '\0';

	while (
			((ifaces[if_count] = strsep(&buffer, ",")) != NULL)
			&&
			(if_count < MAX_WIFI_CARDS)
	) {
		dup = 0;
		for (i = 0; i < if_count; i++)
		{
			if (!strcmp(ifaces[i], ifaces[if_count]))
				dup = 1;
		}
		if (dup)
		{
			continue;
		}
		if (setup_wifi_card(ifaces[if_count], &(wi[if_count])))
		{
			free(buf);
			return -1;
		}
		if_count++;
	}
	free(buf);
	return if_count;
}
