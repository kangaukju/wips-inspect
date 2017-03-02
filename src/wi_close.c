/*
 * wi_test.c
 *
 *  Created on: 2016. 12. 8.
 *      Author: root
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <getopt.h>
#include <signal.h>

#include "wifi_setup.h"


int main(int argc, char **argv)
{
	struct wif *wi = wi_open(argv[1]);
	if (wi == NULL) {
		perror("wi_open");
		return -1;
	}
	wi_close(wi);

	return 0;
}
