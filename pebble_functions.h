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

    /**
     * Function to reset the leader election algorithm.
     */
    void leader_election_reset();
    /**
     * Function to init the leader structures
     */
    void leader_init();
    /**
     * Leader main algorithm
     * @param broadcast Broadcast channel to send data.
     * @return 1 if the execution is over, 0 if the leader has something else to do.
     */
    uchar leader_run(struct broadcast_conn *broadcast);
    /**
     * Function to init each agent data structure
     */
    void agent_init();
    /**
     * Function to check if all the agent have been leader
     * @return 1 if all the agents have been leader 0 otherwise.
     */
    uchar all_been_leader();
    /**
     * Function to check if all the bids have been received
     * @return 1 if all have been received 0 otherwise.
     */
    uchar check_all_bids_rec();
    /**
     * Function to close the leadership
     * @param broadcast Broadcast channel to send data.
     */
    void leader_close(struct broadcast_conn *broadcast);
    /**
     * Function to manage the pebble request
     * @param broadcast Broadcast channel to send data
     * @param from The sender of the request
     * @param rUid Unique id of the received package
     */
    void manage_pebble_request(struct broadcast_conn *broadcast,uchar from,uint16 rUid);
    /**
     * Function to manage the pebble found package
     * @param broadcast Broadcast channel to send data
     * @param from The sender of the request
     */
    void manage_pebble_found(struct broadcast_conn *broadcast,uchar from);
    /**
     * Function to manage the pebble not found
     * @param broadcast Broadcast channel to send data
     * @param from The sender of the request
     */
    void manage_pebble_not_found(struct broadcast_conn *broadcast, uchar from);
    /**
     * Function to manage the package take back your pebbles
     * @param from The sender of the request
     */
    void manage_take_back_pebbles(uchar from);
    /**
     * Function to manage the package take back a pebble
     * @param from The sender of the request
     */
    void manage_send_back_pebble(uchar from);
#ifdef	__cplusplus
}
#endif

#endif	/* PEBBLE_FUNCTIONS_H */

