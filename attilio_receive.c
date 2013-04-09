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

#include "contiki.h"
#include "net/rime.h"
#include "random.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "packages_comm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "net/netstack.h"
#include "pebble_functions.h"
#include "pebble_globals.h"
#include <math.h>



static struct broadcast_conn broadcast;

/**
 * Function to retrieve the global ID
 * @param from Rime address of the node used to retrieve the global ID
 * @return Return the global ID
 */
static uchar get_id(rimeaddr_t *from) {
    uchar i;

    for (i = 0; i < TOT_NUM_NODES; i++) {
        if (rimeaddr_cmp(&nodes_addr_list[i], from))
            return i;
    }
    return 255;
}

/*static uchar is_directed_to_me(rimeaddr_t *from)
{
        if(adj_matrix==NULL)
                return 0;
        uchar from_id=get_id(from);
        if(adj_matrix[MY_ID*TOT_NUM_NODES+from_id]==1)
                return 1;
}
 */

/**
 * Function to initialize the global list of addresses.
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

/*
 * Main process
 */

/*---------------------------------------------------------------------------*/
PROCESS(pebble_process, "Pebble Rigidity Process");
/*Autostarting the process*/
AUTOSTART_PROCESSES(&pebble_process);

/*---------------------------------------------------------------------------*/

/*\TODO: cut the switch using a case handler foreach case. Standard input
 and output structures should be written to feed/retrieve them to/from the functions*/

/**
 * Callback function when a packet is received
 * @param c Connection
 * @param from Sender
 */
static void
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from) {
    /*Header of the received packet*/
    pkg_hdr rec_hdr;
    /*Copy the broadcast buffer in the header structure*/
    memcpy(&rec_hdr, packetbuf_dataptr(), sizeof (pkg_hdr));

    uchar bid, destination_id, sender_id, received_unique_id, received_ind_set_size;
    /*Switch on the type of pkg*/
    switch (rec_hdr.type) {

            /*PKG to start the algorithms*/
        case START_PKG:
	PRINTD ("Start flag received by agent %d\n",NODE_ID);
            /*Set flag and send an event*/
            START_FLAG = 1;
            /*Send the event to unlock the main process*/
            process_post(&pebble_process, PROCESS_EVENT_MSG, NULL);
            //leds_toggle(LEDS_ALL);
            break;
            /*PKG to stop the algorithms*/
        case STOP_PKG:
            /*Set flag and send an event*/
            START_FLAG = 0;
            /*Send the event to unlock the main process*/
            process_post(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
        case ADJ_MATR_PKG:
	PRINTD ("Adj Matrix received by agent %d\n",NODE_ID);
            ADJ_FLAG = 1;
            memcpy(adj_matrix, packetbuf_dataptr() + sizeof (pkg_hdr), TOT_NUM_NODES * TOT_NUM_NODES);
            /*Send the event to unlock the main process*/
            process_post(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
        case LEADER_BID_PKG:

            memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            memcpy(&bid, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
	    PRINTD ("Leader bid received by agent %d from %d amount %d\n",NODE_ID,sender_id,bid);
            received_leader_bid[sender_id] = 1;
            if (max_bid < bid) {
                max_bid = bid;
                max_id = sender_id;
            }
            //Check if i-th agent hasn't been a leader and has the highest bid
            //(it doesn't receive its own packet)
            if (NODE_ID > max_bid && !been_leader) {
                max_bid = NODE_ID;
                max_id = NODE_ID;
            }

            process_post(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
        case LEADER_START_ELECTION_PKG:
	    PRINTD ("Leader start election received by agent %d\n",NODE_ID);
            leader_election_init();
            LEADER_INIT_EL = 1;
            process_post(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
        case IND_SET_PKG:
	    memcpy(&received_ind_set_size, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
	    num_ind_set = received_ind_set_size;
            PRINTD ("Ind Set received by agent %d amount %d\n",NODE_ID,num_ind_set);
            process_post(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
        case REQUEST_PEBBLE_PKG:
            memcpy(&destination_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            if (destination_id == NODE_ID) {
                memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
                memcpy(&received_unique_id, packetbuf_dataptr() + sizeof (pkg_hdr) + 2 * sizeof (uchar), sizeof (uint16));
                PRINTD ("Requested pebble to agent %d from agent %d with ID %d\n",NODE_ID,sender_id,received_unique_id);
                manage_pebble_request(&broadcast, sender_id, received_unique_id);
            }

            break;
        case PEBBLE_FOUND_PKG:
            memcpy(&destination_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
            if (destination_id == NODE_ID) {
	        PRINTD ("Received a pebble found by agent %d from agent %d\n",NODE_ID,sender_id);
                manage_pebble_found(&broadcast, sender_id);
            }
            process_post(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
        case PEBBLE_NOT_FOUND_PKG:
            memcpy(&destination_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            if (destination_id == NODE_ID) {
                memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
	        PRINTD ("Received a pebble not found by agent %d from agent %d\n",NODE_ID,sender_id);
                manage_pebble_not_found(&broadcast, sender_id);
            }
            process_post(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;

        case SEND_BACK_PEBBLE_PKG:
            memcpy(&destination_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            if (destination_id == NODE_ID) {
	    PRINTD ("Sent back a pebble to agent %d\n",NODE_ID);
                pebbles++;
            }
            break;
        case NOTIFY_RIGIDITY_PKG:
            memcpy(&is_rigid, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
	    PRINTD ("Rigidity notification %d\n",is_rigid);
            is_over = 1;
            process_post(&pebble_process, PROCESS_EVENT_MSG, NULL);
            break;
        case TAKE_BACK_PEBBLES_PKG:
            memcpy(&destination_id, packetbuf_dataptr() + sizeof (pkg_hdr), sizeof (uchar));
            if (destination_id == NODE_ID) {
                memcpy(&sender_id, packetbuf_dataptr() + sizeof (pkg_hdr) + sizeof (uchar), sizeof (uchar));
	        PRINTD ("Take back pebbles to agent %d from %d\n",NODE_ID,sender_id);
                manage_take_back_pebbles(sender_id);
            }
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
    //Get the global ID
    NODE_ID = get_id(&rimeaddr_node_addr);
    /*Start only when START_PKG has been received*/
    PROCESS_WAIT_EVENT_UNTIL(START_FLAG);
    /*Wait for the adjacency matrix to be received*/
    PROCESS_WAIT_EVENT_UNTIL(ADJ_FLAG);
    //Init each agent
    agent_init();
    
    /*Main loop*/
    //iterate until or all the agents have been leader or the algorithm is over due
    //to the graph rigidity.
    while (!all_been_leader() && !is_over) {
      
        //Init the structures for the leader election
        leader_election_init();
        //Wait for all the start pkgs to arrive
        PROCESS_WAIT_EVENT_UNTIL(LEADER_INIT_EL || is_over);
        if(is_over)
            continue;
        //Once the start has been received, set the flag to zero for future auctions
        LEADER_INIT_EL = 0;
        //Wait for the start to be notified to all the nodes
        etimer_set(&et, CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        //Desync the agents
        etimer_set(&et, (NODE_ID + 1) *500);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        //If the i-th agent has been a leader, set the bid to the lowest value
        if (been_leader)
            send_leader_bid_pkg(&broadcast, NODE_ID, 0);
        else
            send_leader_bid_pkg(&broadcast, NODE_ID, NODE_ID);

        //If all the bids have arrived
        while (!check_all_leader_pkgs_rec()) {
            PROCESS_WAIT_EVENT();
        }
        //Clear the receiverd_leader_bid
        memset(received_leader_bid, 0, TOT_NUM_NODES);
        //Fill the been leader tab with the new leader ID
        been_leader_tab[max_id] = 1;
        //Debug TODO:remove
        printf("Max id:%d\n", max_id);
        //If the i-th agent is the leader..
        if (max_id == NODE_ID) {

            //Init the leadership structures
            leader_init();

            while (!leader_run(&broadcast)){
		 etimer_set(&et, 50);
        	 PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
	    }

            //Terminate the leadership phase
            leader_close(&broadcast);
            //Wait to send the leader election packet
            etimer_set(&et, CLOCK_SECOND);
            PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
            //Start new election process
            send_leader_election_pkg(&broadcast);
        }
        if (is_over)
            continue;

    }

    if (!is_rigid)
        leds_on(LEDS_BLUE);
    else
        leds_on(LEDS_YELLOW);

    /*End the process*/
    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
