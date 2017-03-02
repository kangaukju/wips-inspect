/*
 * set_channel.c
 *
 *  Created on: 2016. 12. 10.
 *      Author: root
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <getopt.h>
#include <string.h>

#include "osdep/osdep.h"
#include "osdep/common.h"


int check_wi_dev(char* ifname, uint8_t channel)
{
	struct wif *wi = NULL;

	wi = wi_open(ifname);
	if (wi == NULL)
	{
		fprintf(stderr, "failed to open wifi - %s\n", ifname);
		return -1;
	}

	if (channel > 0)
	{
		if (wi_set_channel(wi, channel))
		{
			fprintf(stderr,
					"failed to change %d channel - %s\n", channel, ifname);
			wi_close(wi);
			return -1;
		}
		else
		{
			fprintf(stdout, "success changed channel - %d\n", channel);
		}
	}

	wi_close(wi);
	return 0;
}

void usage()
{
	printf("set_channel\n");
	printf("\t -i interface name\n");
	printf("\t -c channel\n");
	printf("\t -? -h help");
	printf("\n");
}

int main(int argc, char** argv)
{
	int opt;
	char *ifname = NULL;
	uint8_t channel = 0;

	while ((opt = getopt(argc, argv, "i:c:?h")) != -1)
	{
		switch (opt)
		{
		case 'i':
			ifname = strdup(optarg);
			break;
		case 'c':
			channel = atoi(optarg);
			break;
		default:
			usage();
			exit(1);
		}
	}
	if (!ifname || channel == 0)
	{
		usage();
		exit(1);
	}

	if (check_wi_dev(ifname, channel) == -1)
	{
		fprintf(stderr, "[Failure] set channel\n");
	}
	else
	{
		fprintf(stderr, "[Success] set channel\n");
	}
	free(ifname);
}

