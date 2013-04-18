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

#include <math.h>
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


/*Broadcast connection structure*/
static struct broadcast_conn broadcast;
/*Structure representing a start algorithm event*/
static process_event_t start_alg_event;
/*Structure representing that the adjacency matrix has been received*/
static process_event_t adj_pkg_event;
/*Structure representing the event triggering the leader election auction*/
static process_event_t leader_election_event;

//AUXILIARY FUNCTIONS - BEGIN

/**
 * Function to initialize the global list of addresses.
 * Cannot do this from file because I cannot upload file on the MicaZ
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

    nodes_addr_list[8].u8[0] = 9;
    nodes_addr_list[8].u8[1] = 0;

    nodes_addr_list[9].u8[0] = 10;
    nodes_addr_list[9].u8[1] = 0;






    /*/
     * REAL HARDWARE MAC ADDRESSES
            
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

//AUXILIARY FUNCTIONS - END

/*
 * Main process
 */

/*---------------------------------------------------------------------------*/
PROCESS(pebble_process, "Pebble Rigidity Process");
/*Autostarting the process*/
AUTOSTART_PROCESSES(&pebble_process);

/*---------------------------------------------------------------------------*/

/**
 * Callback function when a broadcast packet is received
 * @param c Broadcast Connection
 * @param from Sender
 */
static void
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from) {
    /**
     * Header of the received packet
     * @see pebble_comm.h
     */
    pkg_hdr rec_hdr;
    /*Copy the broadcast buffer in the header structure*/
    memcpy(&rec_hdr, packetbuf_dataptr(), sizeof (pkg_hdr));
    /*Temporary variables exctracted from the packages*/
    uchar bid, destination_id, sender_id, received_unique_id, size_ind_set;
    /*Switch on the type of pkg*/
    switch (rec_hdr.type) {
            /*PKG to start the algorithms*/
        case START_PKG:
            PRINTD("Start flag received by agent %d\n", NODE_ID);
            /*Send the event to unlock the main process*/
            process_post_synch(&pebble_process, start_alg_event, NULL);
            break;
            /*PKG with the adjacency matrix*/
        case ADJ_MATR_PKG:
            PRINTD("Adj Matrix received by agent %d\n", NODE_ID);
            memcpy(adj_matrix, packetbuf_dataptr() + sizeof (pkg_hdr), TOT_NUM_NODES * TOT_NUM_NODES);
            /*Send the event to unlock the main process*/
            process_post_synch(&pebble_process, adj_pkg_event, NULL);
            break;
            /*PKG to start the leader auction*/
        case LEADER_START_ELECTION_PKG:
            PRINTD("Leader start election received by agent %d\n", NODE_ID);
            process_post_synch(&pebble_process, leader_election_event, NULL);
            break;
            /*PKG containing the bid
             *TODO: INSERT THE IND_SET_SIZE
             */
        case LEADER_BID_PKG:
            memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            memcpy(&bid, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
            memcpy(&size_ind_set, packetbuf_dataptr() + sizeof (pkg_hdr) + 2 * sizeof (uchar), sizeof (uchar));
            PRINTD("Leader bid received by agent %d from %d amount %d\n", NODE_ID, sender_id, bid);
            /*Set that it was received*/
            received_leader_bid[sender_id] = 1;
            /*If the bid is geq than the possessed one... store it*/
            if (MAX_BID < bid) {
                MAX_BID = bid;
                MAX_ID = sender_id;
            }
            //Check if i-th agent hasn't been a leader and has the highest bid
            //(it doesn't receive its own packet)
            if (NODE_ID > MAX_BID && !BEEN_LEADER) {
                MAX_BID = NODE_ID;
                MAX_ID = NODE_ID;
            }
            //Check the received independent set size
            NUM_IND_SET = (NUM_IND_SET < size_ind_set) ? size_ind_set : NUM_IND_SET;
            process_post_synch(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
            /*Pkg containing a request of a pebble*/
        case REQUEST_PEBBLE_PKG:
            memcpy(&destination_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            /*The communication is unidirectional. I avoided to use the runicast because of the 
             deadlock issue on the communication. So, if the package is not adressed to the current
             agent, the manage... function is not called and the package is not parsed*/
            if (destination_id == NODE_ID) {
                memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
                memcpy(&received_unique_id, packetbuf_dataptr() + sizeof (pkg_hdr) + 2 * sizeof (uchar), sizeof (uint16));
                PRINTD("Requested pebble to agent %d from agent %d with ID %d\n", NODE_ID, sender_id, received_unique_id);
                manage_pebble_request(&broadcast, sender_id, received_unique_id);
            }
            process_post_synch(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
            /*Pkg pebble found*/
        case PEBBLE_FOUND_PKG:
            memcpy(&destination_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
            if (destination_id == NODE_ID) {
                PRINTD("Received a pebble found by agent %d from agent %d\n", NODE_ID, sender_id);
                manage_pebble_found(&broadcast, sender_id);
            }
            process_post_synch(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
            /*Pkg pebble found*/
        case PEBBLE_NOT_FOUND_PKG:
            memcpy(&destination_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            if (destination_id == NODE_ID) {
                memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
                PRINTD("Received a pebble not found by agent %d from agent %d\n", NODE_ID, sender_id);
                manage_pebble_not_found(&broadcast, sender_id);
            }
            process_post_synch(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
            /*Pkg take back a pebble*/
        case SEND_BACK_PEBBLE_PKG:
            memcpy(&destination_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            if (destination_id == NODE_ID) {
                memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
                PRINTD("Sent back a pebble to agent %d from %d\n", NODE_ID, sender_id);
                manage_send_back_pebble(sender_id);
            }
            process_post_synch(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
            /*Pkg take back all the pebbles*/
        case TAKE_BACK_PEBBLES_PKG:
            memcpy(&destination_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            if (destination_id == NODE_ID) {
                memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
                PRINTD("Take back pebbles to agent %d from %d\n", NODE_ID, sender_id);
                manage_take_back_pebbles(sender_id);
            }
            process_post_synch(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
            /*Pkg for the notification of the rigidity*/
        case NOTIFY_RIGIDITY_PKG:
            memcpy(&IS_RIGID, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            PRINTD("Rigidity notification %d\n", IS_RIGID);
            if (!IS_RIGID)
                leds_on(LEDS_BLUE);
            else
                leds_on(LEDS_ALL);
            //Once the rigidity check is over, kill the main process
            process_exit(&pebble_process);
            break;
        default:
            printf("Error in switch\n");
            break;
    }

}

static const struct broadcast_callbacks broadcast_call = {broadcast_recv};


/*---------------------------------------------------------------------------*/

/*Main thread of the process*/
PROCESS_THREAD(pebble_process, ev, data) {

    //Timer structure
    static struct etimer et;
    //Set the exit handler
    PROCESS_EXITHANDLER(broadcast_close(&broadcast);)
    //Begin the process
    PROCESS_BEGIN();
    //Open the broadcast channel on 129 and set the callback function
    broadcast_open(&broadcast, 129, &broadcast_call);
    //Init the address list statically
    set_addr_list();

    /*Start only when START_PKG has been received*/
    PROCESS_WAIT_EVENT_UNTIL(ev == start_alg_event);
    /*Wait for the adjacency matrix to be received*/
    PROCESS_WAIT_EVENT_UNTIL(ev == adj_pkg_event);
    //Init each agent
    agent_init();
    //Init the structures for the leader election
    leader_election_reset();
    /*Main loop*/
    //iterate until or all the agents have been leader or the algorithm is over due
    //to the graph rigidity.
    while (!all_been_leader()) {
        //The previous leader is not receiving the leader election event. So a flag
        //to understand if the current agent was the the leader in the previous loop of
        //The algorithm is required
        if (PREV_LEADER) {
            PREV_LEADER = 0;
        } else {
            //Wait for all the start pkgs to arrive
            PROCESS_WAIT_EVENT_UNTIL(ev == leader_election_event);
        }

        leader_election_reset();

        //Wait for the start to be notified to all the nodes
        //Desync the agents
        etimer_set(&et, (NODE_ID + 1) *20 * SCALE);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        //If the i-th agent has been a leader, set the bid to the lowest value,
        //otherwise set it to 1
        if (BEEN_LEADER)
            send_leader_bid_pkg(&broadcast, NODE_ID, 0, NUM_IND_SET);
        else
            send_leader_bid_pkg(&broadcast, NODE_ID, NODE_ID, NUM_IND_SET);
        //If all the bids have arrived (for the sake of reliability, retransmissions
        //should be inserted but problems related to transmission collisions could
        //arise).
        while (!check_all_bids_rec()) {
            PROCESS_WAIT_EVENT();
        }
        //Fill the been leader tab with the new leader ID
        been_leader_tab[MAX_ID] = 1;
        //If the i-th agent is the leader..
        if (MAX_ID == NODE_ID) {
            //Init the leadership structures
            leader_init();
            while (!leader_run(&broadcast)) {
                etimer_set(&et, 10 * SCALE);
                PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
            }
            //Terminate the leadership phase
            leader_close(&broadcast);
            //Start new election process
            send_leader_election_pkg(&broadcast);
        }
    }

    /*End the process*/
    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
