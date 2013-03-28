#ifndef _PACKAGES_H_
#define _PACKAGES_H_


#include "contiki.h"
#include "net/rime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef unsigned char uchar;
typedef struct{
	unsigned char type;
	rimeaddr_t receiver;
	unsigned char data_len;
}pkg_hdr;

enum{
	START_PKG=1,
	STOP_PKG,
	ADJ_MATR_PKG,
	RYAN_PKG,
	CHANGE_LED,
	TOKEN_PKG
};

void send_start_pkg_broad(struct broadcast_conn *broadcast);

void send_stop_pkg_broad(struct broadcast_conn *broadcast);

void send_adj_pkg_broad(struct broadcast_conn *broadcast, uchar n,uchar *adj);

#endif

