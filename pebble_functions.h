/* 
 * File:   pebble_functions.h
 * Author: sickboy
 *
 * Created on April 2, 2013, 1:12 PM
 */

#ifndef PEBBLE_FUNCTIONS_H
#define	PEBBLE_FUNCTIONS_H
#include "pebble_globals.h"
#include "packages.h"
#include "contiki.h"
#include "net/rime.h"
#include "random.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"

#ifdef	__cplusplus
extern "C" {
#endif

    extern uchar LEADER_INIT_EL;

    extern uchar received_leader_bid[TOT_NUM_NODES];

    void leader_election_init();

    void leader_init();

    void agent_init();

    uchar all_been_leader();

    uchar check_all_leader_pkgs_rec();

    void leader_close(struct broadcast_conn *broadcast);
    
#ifdef	__cplusplus
}
#endif

#endif	/* PEBBLE_FUNCTIONS_H */

