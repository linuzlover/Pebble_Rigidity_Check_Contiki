#ifndef _PACKAGES_H_
#define _PACKAGES_H_

#include "contiki.h"
#include "net/rime.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
	unsigned char type;
	unsigned char receiver;
	unsigned char data_len;
}pkg_hdr;

enum{
	START_PKG=1,
	STOP_PKG,
	ADJ_MATR_PKG,
	RYAN_PKG,
	CHANGE_LED
};

#endif

