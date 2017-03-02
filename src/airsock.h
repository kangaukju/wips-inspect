/*
 * airsock.h
 *
 *  Created on: 2015. 5. 4.
 *      Author: root
 */

#ifndef AIR_COMM_SOCK_H_
#define AIR_COMM_SOCK_H_

#include <stddef.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/time.h>
#include <time.h>

int recv_t(int sock, uint8_t *buf, size_t buflen, struct timeval wait, int flag);
int send_t(int sock, uint8_t *buf, size_t buflen, struct timeval wait, int flag);
int recv_u(int sock, uint8_t *buf, size_t buflen, struct timeval wait);
int send_u(int sock, uint8_t *buf, size_t buflen, struct timeval wait);
int recvfrom_u(int sock, uint8_t *buf, size_t buflen, struct timeval wait, struct sockaddr_in *from);
int sendto_u(int sock, uint8_t *buf, size_t buflen, struct timeval wait, struct sockaddr_in *to);
int accept_t(int sock, struct timeval wait, struct sockaddr_in *clientaddr);
int server_t(const char* ipaddr, int port, int blocking);
int server_u(const char* ipaddr, int port, int blocking);
int connect_u(const char *src, const char *dst, uint16_t port, int blocking);
int connect_t(const char *src, const char *dst, uint16_t port, int blocking);
int set_keepalive(int sock, int keep_count, int keep_idle, int keep_interval);
int set_rst_close(int sock);

/* Unix Domain Socket */
int unix_connect_u(const char *sockfile, int blocking);
int unix_connect_t(const char *sockfile, int blocking);
int unix_server_t(const char* sockfile, int blocking);
int unix_server_u(const char* sockfile, int blocking);
int unix_accept_t(int sock, struct timeval wait, struct sockaddr_un *clientaddr);

#endif /* AIR_COMM_SOCK_H_ */
