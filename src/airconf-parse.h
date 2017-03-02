/*
 * airconf-parse.h
 *
 *  Created on: 2016. 12. 7.
 *      Author: root
 */

#ifndef SRC_XML_AIRCONF_PARSE_H_
#define SRC_XML_AIRCONF_PARSE_H_

#include <unistd.h>
#include <stdio.h>
#include <sys/time.h>
#include "llist.h"
#include "common.h"
#include "osdep/osdep.h"

typedef struct addr_conf_any {
	mac_t(addr);
} addr_conf_any;

typedef struct addr_conf_ap {
	mac_t(ap);
} addr_conf_ap;

typedef struct addr_conf_sta {
	mac_t(sta);
} addr_conf_sta;

typedef struct addr_conf_1 {
	mac_t(da);
} addrconf_1;

typedef struct addr_conf_2 {
	mac_t(da);
	mac_t(sa);
} addrconf_2;

typedef struct addr_conf_3 {
	mac_t(da);
	mac_t(sa);
	mac_t(bssid);
} addrconf_3;

typedef struct addr_conf_4 {
	mac_t(da);
	mac_t(sa);
	mac_t(ta);
	mac_t(ra);
} addrconf_4;

#define cv_def(name, type) \
    type name; \
    uint8_t name##_en;

#define cv_arr_def(name, type, size) \
    type name[size]; \
    uint8_t name##_en;

#define cv_is_enable(name) \
    (name##_en == 1)

#define cv_set_enable(name) \
	do { name##_en = 1; } while (0)

#define cv_set_disable(name) \
	do { name##_en = 0; } while (0)

struct tagconf {
	struct tagconf *next;
	uint8_t tag;
	uint8_t len;
	uint8_t *data;
};

struct airconf {
	uint8_t pktbuf[4096];
	int pktbuf_len;
	char profileid[128];
	char configid[128];

	// config section
	uint8_t enable;
	uint32_t key;
	cv_arr_def(desc, char, 128);
	/* config sequence - seq 번호 순서대로 진행한다. */
	cv_def(seq, uint32_t);
	cv_def(magic_key, uint32_t);
	cv_def(version, uint8_t);
	cv_def(channel, uint8_t);
	cv_def(type, uint8_t);
	cv_def(subtype, uint8_t);
	cv_def(tods, uint8_t);
	cv_def(fromds, uint8_t);
	cv_def(protect, uint8_t);
	cv_def(pwr_sleep, uint8_t);
	cv_def(duration, uint16_t);
	/* aircapture: channel dwell time - millisecond */
	/* airshooter: channel shoot interval - millisecond */
	cv_def(dwell, uint32_t);
	/* aircapture: capture start delay - millisecond */
	/* airshooter: first shoot delay - millisecond */
	cv_def(delay, long);
	cv_def(deauth_reason, uint16_t);
	cv_def(data_size, int);
	cv_arr_def(data_sign, char, 32);
	cv_def(addr_count, uint8_t);
	cv_def(addr1, addrconf_1);
	cv_def(addr2, addrconf_2);
	cv_def(addr3, addrconf_3);
	cv_def(addr4, addrconf_4);
	cv_def(ap_addr, addr_conf_ap);
	cv_def(sta_addr, addr_conf_sta);
	cv_def(any_addr, addr_conf_any);
	cv_def(pcap, uint8_t);
	struct tagconf *tags;
	struct timeval shooter_tv;
	struct timeval capture_tv;
	struct airconf *next;
	struct airconf *prev;
};

struct sqlairconf {
	struct sqlairconf *next;
	char profileid[128];
	char configid[128];
	char capturexml[64];
	char shooterxml[64];
};

// API tagconf
void tagconf_print(struct tagconf *tag);
void tagconf_simple_print(struct tagconf *tag);
struct tagconf* tagconf_find(struct tagconf *first, uint8_t id);
struct tagconf* tagconf_vendor_find(struct tagconf *first, uint8_t *oui);
struct tagconf* tagconf_new(struct tagconf *prev, uint8_t tag, uint8_t len, uint8_t *data);
struct tagconf* tagconf_clone(struct tagconf *tag);
struct tagconf* tagconf_last(struct tagconf *tag);
struct tagconf* tagconf_sort(struct tagconf *tag);
int tagconf_free(struct tagconf *first);

// API airconf
void airconf_print(struct airconf *conf);
void airconf_free(struct airconf *conf);
void airconf_filelist(llist **files, const char *basedir);
struct airconf* airconf_sort(struct airconf* head);
uint32_t airconf_count(struct airconf* head);
struct airconf* airconf_find_keyseq(struct airconf* head, uint32_t key, uint32_t seq);
struct airconf* airconf_find_key(struct airconf* head, uint32_t key);
struct airconf* airconf_from_file(const char *filename, int debug);
struct airconf* airconf_from_buf(char *xmlbuf, size_t len, int debug);
struct airconf* airconf_last(struct airconf *conf);
int airconf_from_path(struct airconf** head, const char *basedir, int debug);
uint32_t max_dwell(struct airconf *conf);
uint32_t min_dwell(struct airconf *conf);

// API sqlairconf
struct sqlairconf* sqlairconf_load(const char *dbfile, const char *profileid);
void sqlairconf_free(struct sqlairconf *conf);


#endif /* SRC_XML_AIRCONF_PARSE_H_ */
