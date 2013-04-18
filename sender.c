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
#include "net/netstack.h"
#include "dev/leds.h"
#include "packages_comm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>




/*---------------------------------------------------------------------------*/
//Broadcast example process. I am keeping the former name
PROCESS(example_broadcast_process, "Broadcast example");
AUTOSTART_PROCESSES(&example_broadcast_process);

/*---------------------------------------------------------------------------*/
//\TODO: Empty... this code will act also as a packet sniffer so it will be implemented later

static void
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from) {
    //This will represent our logger in the real hardware experiments
}

//Not rigid 7 agents
//*/
static uchar temp_adj_matrix[TOT_NUM_NODES - 1][TOT_NUM_NODES - 1] = {
    {0, 1, 0, 1, 1, 1, 1},
    {1, 0, 1, 1, 1, 0, 0},
    {0, 1, 0, 1, 0, 1, 0},
    {1, 1, 1, 0, 1, 1, 0},
    {1, 1, 0, 1, 0, 1, 0},
    {1, 0, 1, 1, 1, 0, 0},
    {1, 0, 0, 0, 0, 0, 0}};
//*/

//Rigid 6 agents
/*/
static uchar temp_adj_matrix[TOT_NUM_NODES - 1][TOT_NUM_NODES - 1] = {
    {0, 1, 0, 1, 1, 1},
    {1, 0, 1, 1, 1, 0},
    {0, 1, 0, 1, 0, 1},
    {1, 1, 1, 0, 1, 1},
    {1, 1, 0, 1, 0, 1},
    {1, 0, 1, 1, 1, 0}};
//*/

static void set_adj_matrix(uchar *adj) {
    uchar i, j;

    for (i = 0; i < TOT_NUM_NODES - 1; i++) {
        for (j = 0; j < TOT_NUM_NODES - 1; j++) {
            if (temp_adj_matrix[i][j]) {
                adj[mat2vec(i, j)] = 1;
            }
        }
    }
}

//Callbacks and broadcast connection structure
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
struct broadcast_conn broadcast;

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_broadcast_process, ev, data) {
    static struct etimer et;
    pkg_hdr to_send;
    //Adjacency matrix to be sent
    uchar adj[TOT_NUM_NODES * TOT_NUM_NODES];
    //Filling it
    memset(adj, 0, TOT_NUM_NODES * TOT_NUM_NODES);
    set_adj_matrix(adj);
    

    //Setting handlers and begin
    PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

    PROCESS_BEGIN();

    //Send start pkg in broadcast to all the agents
    broadcast_open(&broadcast, 129, &broadcast_call);
    etimer_set(&et, 5 * CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    send_start_pkg_broad(&broadcast);

    //Send the adjacency matrix in broadcast to all the agents
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    send_adj_pkg_broad(&broadcast, adj);
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    //Send the token to the first agent
    to_send.type = LEADER_START_ELECTION_PKG;
    //to_send.receiver = dest;
    packetbuf_clear();
    packetbuf_copyfrom(&to_send, sizeof (pkg_hdr));
    broadcast_send(&broadcast);
    //Turn off something
    //NETSTACK_MAC.off(0);
    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
