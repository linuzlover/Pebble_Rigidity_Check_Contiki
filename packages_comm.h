//Pebble Game Rigidity Test. The goal of this software is to implement the pebble
//game rigidity check on the MicaZ devices under contiki-os.
//Copyright (C) 2013 Attilio Priolo <priolo@dia.uniroma3.it>
//
//This file is part of Pebble Game Rigidity Test.
//
//Pebble Game Rigidity Test is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 2 of the License, or
//(at your option) any later version.
//
//Pebble Game Rigidity Test is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with Foobar. If not, see <http://www.gnu.org/licenses/>.


#ifndef _PACKAGES_H_
#define _PACKAGES_H_

/*Including C libraries*/

#include "contiki.h"
#include "net/rime.h"
#include "pebble_globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * \var nodes_addr_list List of the nodes addresses in global order.
 */

extern rimeaddr_t nodes_addr_list[TOT_NUM_NODES];

/*! 
 * \struct pkg_hdr
 * This struct is intended to contain the header of the package to be sent from/to
 * the MicaZ nodes. 
 */
typedef struct {
    /**
     * \var type Defines the type of the package (according to the enum)
     */
    uchar type;

    //TODO: Improve the header
} pkg_hdr;

/**
 *\enum type Enum describing the type of package to be sent/received
 */
enum {
    //Pkg to start the algorithms
    START_PKG = 1,
    //Pkg to send the desired ajacency matrix
    ADJ_MATR_PKG,
    //Pkg to model the bid in the leader election process
    LEADER_BID_PKG,
    //Pkg to start the leader election
    LEADER_START_ELECTION_PKG,
    //Pkg to notify the rigidity condition to all the agents
    NOTIFY_RIGIDITY_PKG,
    //Pkg to request a pebble to a neighboring agent
    REQUEST_PEBBLE_PKG,
    //Pkg to send back a pebble to an agent
    SEND_BACK_PEBBLE_PKG,
    //Pkg to notify that a pebble has been found
    PEBBLE_FOUND_PKG,
    //Pkg to notify that a pebble hasn't been found
    PEBBLE_NOT_FOUND_PKG,
    //Pkg to inform an agent to get back its pebbles :-)
    TAKE_BACK_PEBBLES_PKG,
    //Pkg to ask to your neighbor if the indipendent set contains this edge
    CHECK_YOUR_IS_PKG,
    //Response package
    CHECK_YOUR_IS_RES_PKG
};

/**
 * This function sends the start package in broadcast
 * @param broadcast Broadcast channel
 */
void send_start_pkg_broad(struct broadcast_conn *broadcast);
/**
 * This function sends the stop package in broadcast
 * @param broadcast Broadcast channel
 */
void send_stop_pkg_broad(struct broadcast_conn *broadcast);
/**
 * This function sends the adjacenty matrix in broadcast
 * @param broadcast Broadcast channel
 * @param adj Adjacency matrix represented as a single vector of TOT_NUM_NODES^2 elements
 */
void send_adj_pkg_broad(struct broadcast_conn *broadcast, uchar *adj);
/**
 * This function sends the token package to the i-th agent neighbors according to the adjacency
 * matrix adj.
 * 
 * @param broadcast Broadcast channel
 * @param i i-th agent index
 * @param adj Adjacency matrix represented as a single vector of TOT_NUM_NODES^2 elements
 * @param nodes_addr_list List of the nodes address (\TODO: make it dynamic). Basically this variable is
 * a static array of rimeaddr_t TOT_NUM_NODES elements. The position in the array gives the short global index
 * of the agent (the "i").
 */
void send_token_pkg(struct broadcast_conn *broadcast, uchar i, uchar *adj, rimeaddr_t *nodes_addr_list);
/**
 * Send the leader election start pkg
 * @param broadcast Broadcast channel
 */
void send_leader_election_pkg(struct broadcast_conn *broadcast);
/**
 * This function send the "bid" from agent "id" to the others for the leader election auction.
 * @param broadcast Broadcast channel
 * @param id ID of the sender
 * @param bid Bid for the auction
 */
void send_leader_bid_pkg(struct broadcast_conn *broadcast, uchar id, uchar bid, uchar size_ind_set);
/**
 * This function sends the rigidity notification in broadcast
 * @param broadcast Broadcast channel
 * @param rigidity 1 if the communication graph is rigid, 0 otherwise
 */
void send_rigidity_pkg(struct broadcast_conn *broadcast, uchar rigidity);
/**
 * This function sends a pebble request from agent "from" adressed to agent "to"
 * @param broadcast Broadcast channel
 * @param to The ID the pebble request is addressed to
 * @param from The ID the pebble request comes from
 * @param uId Unique identifier for the package 
 */
void send_pebble_request_pkg(struct broadcast_conn *broadcast, uchar to, uchar from, uint16 uId);
/**
 * This functions sends back a pebble to agent "to"
 * @param broadcast Broadcast channel
 * @param to Agent to send the pebble back to
 */
void send_back_pebble_pkg(struct broadcast_conn *broadcast, uchar to);
/**
 * This function sends a pebble found or a pebble not found msg
 * @param broadcast Broadcast channel
 * @param to The destination ID
 * @param from The sender ID
 * @param found 1 if a pebble found has to be sent, 0 otherwise
 */
void send_pebble_msg(struct broadcast_conn *broadcast, uchar to, uchar from, uchar found);
/**
 * This function sends back all the pebbles to the destination
 * @param broadcast Broadcast channel
 * @param to The destination ID
 * @param from The sender ID
 */
void send_take_back_pebbles(struct broadcast_conn *broadcast, uchar to, uchar from);

void send_check_is_pkg(struct broadcast_conn *broadcast, uchar to,uchar from);

void send_check_is_res_pkg(struct broadcast_conn *broadcast, uchar to,uchar from,uchar response);


#endif
