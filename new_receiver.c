/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
 * Modified by Attilio Priolo, University of Roma Tre
 * <priolo@dia.uniroma3.it>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *
 */

/**
 * \file
 *         Implementing the Pebble Rigidity Check over Rime
 * \author
 *         Attilio Priolo <priolo@dia.uniroma3.it>
 * Based on the original work by
 *         Adam Dunkels <adam@sics.se>
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "contiki.h"
#include "net/rime.h"
#include "random.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "packages_comm.h"
#include "net/netstack.h"
#include "pebble_functions.h"
#include "pebble_globals.h"

//Start the algorithm event
static process_event_t start_algorithm_evt;
//Adjacency matrix received event
static process_event_t adjacency_received_evt;
//Trigger leader election event
static process_event_t start_leader_election_evt;
//Stop everything event (NOT IMPLEMENTED)
static process_event_t stop_evt;

/*
 * Function to reset the leader election procedure. Note that each agent sets its 
 * leader bid as received because it cannot receive its own packets.
 */
void leader_election_reset() {
    memset(received_leader_bid, 0, TOT_NUM_NODES * sizeof (uchar));
    received_leader_bid[NODE_ID] = 1;
    last_leader = 0;
}

/**
 * Function to initialize the global list of addresses.
 * TODO: This function must be replaced by a neighbor discovery procedure.
 */
static void set_addr_list() {

    nodes_addr_list[0].u8[0] = 1;
    nodes_addr_list[0].u8[1] = 0;

    nodes_addr_list[1].u8[0] = 2;
    nodes_addr_list[1].u8[1] = 0;

    nodes_addr_list[2].u8[0] = 3;
    nodes_addr_list[2].u8[1] = 0;

    nodes_addr_list[3].u8[0] = 4;
    nodes_addr_list[3].u8[1] = 0;

    nodes_addr_list[4].u8[0] = 5;
    nodes_addr_list[4].u8[1] = 0;

    nodes_addr_list[5].u8[0] = 6;
    nodes_addr_list[5].u8[1] = 0;

    nodes_addr_list[6].u8[0] = 7;
    nodes_addr_list[6].u8[1] = 0;

    nodes_addr_list[7].u8[0] = 8;
    nodes_addr_list[7].u8[1] = 0;
    /*
        nodes_addr_list[8].u8[0] = 9;
        nodes_addr_list[8].u8[1] = 0;

        nodes_addr_list[9].u8[0] = 10;
        nodes_addr_list[9].u8[1] = 0;
     */





    /*/
            temp.u8[0]=0;
            temp.u8[1]=0;
            nodes_addr_list[0]=temp;

            temp.u8[0]=0;
            temp.u8[1]=15;
            nodes_addr_list[1]=temp;

            temp.u8[0]=6;
            temp.u8[1]=251;
            nodes_addr_list[2]=temp;

            temp.u8[0]=13;
            temp.u8[1]=4;
            nodes_addr_list[3]=temp;

            temp.u8[0]=31;
            temp.u8[1]=70;
            nodes_addr_list[4]=temp;

            temp.u8[0]=83;
            temp.u8[1]=12;
            nodes_addr_list[5]=temp;

            temp.u8[0]=91;
            temp.u8[1]=19;
            nodes_addr_list[6]=temp;

            temp.u8[0]=127;
            temp.u8[1]=108;
            nodes_addr_list[7]=temp;

            temp.u8[0]=128;
            temp.u8[1]=101;
            nodes_addr_list[8]=temp;

            temp.u8[0]=158;
            temp.u8[1]=128;
            nodes_addr_list[9]=temp;

            temp.u8[0]=196;
            temp.u8[1]=115;
            nodes_addr_list[10]=temp;

            temp.u8[0]=212;
            temp.u8[1]=108;
            nodes_addr_list[11]=temp;

            temp.u8[0]=217;
            temp.u8[1]=209;
            nodes_addr_list[12]=temp;

            temp.u8[0]=226;
            temp.u8[1]=100;
            nodes_addr_list[13]=temp;

            temp.u8[0]=255;
            temp.u8[1]=255;
            nodes_addr_list[14]=temp;
	
            temp.u8[0]=255;
            temp.u8[1]=255;
            nodes_addr_list[15]=temp;
    //*/
}

/**
 * Function to retrieve the global ID
 * TODO: This function must be replaced by another one following the neighbors discovery procedure
 * @param from Rime address of the node used to retrieve the global ID
 * @return Return the global ID
 */
static uchar get_id(rimeaddr_t *from) {
    uchar i;

    for (i = 0; i < TOT_NUM_NODES - 1; i++) {
        if (rimeaddr_cmp(&(nodes_addr_list[i]), from))
            return i;
    }
    return 255;
}

/*
 * Main process
 */

/*---------------------------------------------------------------------------*/
PROCESS(pebble_process, "Pebble Rigidity Process");
/*Autostarting the process*/
AUTOSTART_PROCESSES(&pebble_process);

/*---------------------------------------------------------------------------*/
//Unicast connection
static struct runicast_conn runicast;
//Broadcast connection
static struct broadcast_conn broadcast;

/**
 * Callback function triggered when a new packet is received.
 * @param c Broadcast connection
 */
static void
broadcast_recv(struct broadcast_conn *c) {
    //Pkg header
    pkg_hdr rec_hdr;
    /*Copy the broadcast buffer in the header structure*/
    memcpy(&rec_hdr, packetbuf_dataptr(), sizeof (pkg_hdr));

    uchar bid, sender_id, received_ind_set_size;
    /*Switch on the type of pkg*/
    switch (rec_hdr.type) {
            /*PKG to start the algorithms*/
        case START_PKG:
            PRINTD("Start flag received by agent %d\n", NODE_ID + 1);
            /*Send the event to unlock the main process*/
            process_post_synch(&pebble_process, start_algorithm_evt, NULL);
            break;
            /*PKG to stop the algorithms*/
        case STOP_PKG:
            /*Send the event to unlock the main process*/
            process_post_synch(&pebble_process, stop_evt, NULL);
            break;
        case ADJ_MATR_PKG:
            PRINTD("Adj Matrix received by agent %d\n", NODE_ID + 1);
            memcpy(adj_matrix, packetbuf_dataptr() + sizeof (pkg_hdr), TOT_NUM_NODES * TOT_NUM_NODES);
            /*Send the event to unlock the main process*/
            process_post_synch(&pebble_process, adjacency_received_evt, NULL);
            break;
        case LEADER_START_ELECTION_PKG:
            PRINTD("Leader start election received by agent %d\n", NODE_ID + 1);
            process_post_synch(&pebble_process, start_leader_election_evt, NULL);
            break;
        case LEADER_BID_PKG:
            memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            memcpy(&bid, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
            PRINTD("Leader bid received by agent %d from %d amount %d\n", NODE_ID + 1, sender_id + 1, bid);
            received_leader_bid[sender_id] = 1;

            if (max_bid < bid) {
                max_bid = bid;
                max_id = sender_id;
            }
            //Check if i-th agent hasn't been a leader and has the highest bid
            //(it doesn't receive its own packet)
            if (NODE_ID >= max_bid && !been_leader) {
                max_bid = NODE_ID;
                max_id = NODE_ID;
            }
            process_post_synch(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
        case IND_SET_PKG:
            memcpy(&received_ind_set_size, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            num_ind_set = received_ind_set_size;
            PRINTD("Ind Set received by agent %d amount %d\n", NODE_ID, num_ind_set);

            break;
        case NOTIFY_RIGIDITY_PKG:
            memcpy(&is_rigid, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            PRINTD("Rigidity notification %d\n", is_rigid);
            is_over = 1;

            break;
        default:
            printf("Error in broadcast switch\n");
            break;
    }

}
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};

static void
timedout_runicast(struct runicast_conn *c, const rimeaddr_t *to, uchar retransmissions) {
    printf("runicast message timed out when sending to %d.%d, retransmissions %d\n",
            to->u8[0], to->u8[1], retransmissions);

}

static void
sent_runicast(struct runicast_conn *c, const rimeaddr_t *to, uchar retransmissions) {
    printf("runicast message sent to %d.%d, retransmissions %d\n",
            to->u8[0], to->u8[1], retransmissions);
}

static void
recv_runicast(struct runicast_conn *c, const rimeaddr_t *from, uchar seqno) {
    /*Header of the received packet*/
    pkg_hdr rec_hdr;
    /*Copy the broadcast buffer in the header structure*/
    memcpy(&rec_hdr, packetbuf_dataptr(), sizeof (pkg_hdr));

    uchar destination_id, sender_id, received_unique_id;
    /*Switch on the type of pkg*/
    switch (rec_hdr.type) {

        case REQUEST_PEBBLE_PKG:
            memcpy(&destination_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
            memcpy(&received_unique_id, packetbuf_dataptr() + sizeof (pkg_hdr) + 2 * sizeof (uchar), sizeof (uint16));
            PRINTD("Received a pebble request by agent %d from agent %d with ID %d\n", NODE_ID + 1, sender_id + 1, received_unique_id);
            manage_pebble_request(&runicast, sender_id, received_unique_id);
            process_post(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
        case PEBBLE_FOUND_PKG:
            memcpy(&destination_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
            PRINTD("Received a pebble found by agent %d from agent %d\n", NODE_ID + 1, sender_id + 1);
            manage_pebble_found(&runicast, sender_id);

            process_post(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
        case PEBBLE_NOT_FOUND_PKG:
            memcpy(&destination_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
            PRINTD("Received a pebble not found by agent %d from agent %d\n", NODE_ID + 1, sender_id + 1);
            manage_pebble_not_found(&runicast, sender_id);
            process_post(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;

        case SEND_BACK_PEBBLE_PKG:
            memcpy(&destination_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
            PRINTD("Received a pebble back by agent %d from %d\n", NODE_ID + 1, sender_id + 1);
            manage_send_back_pebble(sender_id);
            process_post(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
        case TAKE_BACK_PEBBLES_PKG:
            memcpy(&destination_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
            PRINTD("Received a Take back pebbles by agent %d from %d\n", NODE_ID + 1, sender_id + 1);
            manage_take_back_pebbles(sender_id);
            process_post(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
        default:
            PRINTD("Error in unicast switch\n");
            break;
    }

}

static const struct runicast_callbacks runicast_callbacks = {recv_runicast,
    sent_runicast,
    timedout_runicast};

/*---------------------------------------------------------------------------*/

/*Main thread of the process*/
PROCESS_THREAD(pebble_process, ev, data) {

    //Timer structure
    static struct etimer et;
    //Set the exit handler
    PROCESS_EXITHANDLER(runicast_close(&runicast); broadcast_close(&broadcast));
    //Begin the process
    PROCESS_BEGIN();
    //Init the random library
    random_init(rimeaddr_node_addr.u8[0] + 1);
    //Open the unicast channel on 144 and set the callback function
    runicast_open(&runicast, 145, &runicast_callbacks);
    //Open the trickle channel on 145
    broadcast_open(&broadcast, 129, &broadcast_call);
    //Init the address list statically
    set_addr_list();
    //Get the global ID
    NODE_ID = get_id(&rimeaddr_node_addr);
    /*Start only when START_PKG has been received*/
    PROCESS_WAIT_EVENT_UNTIL(ev == start_algorithm_evt);
    /*Wait for the adjacency matrix to be received*/
    PROCESS_WAIT_EVENT_UNTIL(ev == adjacency_received_evt);
    //Init the agent's data structures
    agent_init();
    /*
    #ifdef DEBUG
        int i, j;

        PRINTD("Printing the adjacency matrix received by the first node (it is assumed to exist)\n");
        if (NODE_ID == 0) {
            for (i = 0; i < TOT_NUM_NODES; i++) {
                for (j = 0; j < TOT_NUM_NODES; j++) {
                    PRINTD("%d\t", adj_matrix[mat2vec(i, j)]);
                }
                PRINTD("\n");
            }
        }
    #endif
     */


    //While all the agents have not been leader or no rigidity notifications
    //has been sent
    while (!all_been_leader() && !is_over) {
        int iter = TOT_NUM_NODES;
        //Wait a start leader auction
        if (!last_leader)
            PROCESS_WAIT_EVENT_UNTIL(ev == start_leader_election_evt);
        else
        {
            send_leader_election_pkg(&broadcast);

        }

        //Reset the leader election structs
        leader_election_reset();

        //Desync the agents
        etimer_set(&et, (NODE_ID + 1)* 25);
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        if (been_leader)
        {
            send_leader_bid_pkg(&broadcast, NODE_ID, 0);
        }
        else
            send_leader_bid_pkg(&broadcast, NODE_ID, NODE_ID);
            

        //Until all the bids have not been received
        while (!check_all_bids_rec()) {
            //Wait 10 ticks to check again
            etimer_set(&et, 10);
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        }
        //Debug TODO:remove
        PRINTD("New Leader:%d\n", max_id + 1);
        //Fill the been leader tab with the new leader ID
        been_leader_tab[max_id] = 1;
        //Consider the new leader
        if (max_id == NODE_ID) {
            //Init the leadership structures
            leader_init();
            etimer_set(&et, 100);
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
            //Close the leadership stage
            leader_close(&broadcast);
        }
        //Reset the bid variables
        max_id = 0;
        max_bid = 0;
        //------------------------
    }

    PRINTD("OVER\n");
    /*End the process*/
    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
