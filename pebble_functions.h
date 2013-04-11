/* 
 * File:   pebble_functions.h
 * Author: sickboy
 *
 * Created on April 2, 2013, 1:12 PM
 */

#ifndef PEBBLE_FUNCTIONS_H
#define	PEBBLE_FUNCTIONS_H
#include "pebble_globals.h"
#include "packages_comm.h"
#include "contiki.h"
#include "net/rime.h"
#include "random.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "pebble_assign_set.h"

#ifdef	__cplusplus
extern "C" {
#endif

    extern uchar LEADER_INIT_EL;

    extern uchar received_leader_bid[TOT_NUM_NODES];
    
    extern uchar uId;

    void leader_election_init();

    void leader_init();
    
    uchar leader_run(struct runicast_conn *c,struct trickle_conn *trick);

    void agent_init();

    uchar all_been_leader();

    uchar check_all_leader_pkgs_rec();

    void leader_close(struct trickle_conn *trick);
    
    void manage_pebble_request(struct runicast_conn *c, uchar from, uint16 rUid);
    
    void manage_pebble_found(struct runicast_conn *c, uchar from);
    
    void manage_pebble_not_found(struct runicast_conn *c, uchar from);
    
    void manage_take_back_pebbles(uchar from);
    
    void manage_send_back_pebble(uchar from);
#ifdef	__cplusplus
}
#endif

#endif	/* PEBBLE_FUNCTIONS_H */

