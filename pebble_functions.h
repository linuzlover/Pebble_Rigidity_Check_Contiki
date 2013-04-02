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

#ifdef	__cplusplus
extern "C" {
#endif

extern uchar received_leader_bid[TOT_NUM_NODES];

void leader_election_init();
    
void leader_init(); 

void agent_init();

uchar all_been_leader();

uchar check_all_leader_pkgs_rec();
#ifdef	__cplusplus
}
#endif

#endif	/* PEBBLE_FUNCTIONS_H */

