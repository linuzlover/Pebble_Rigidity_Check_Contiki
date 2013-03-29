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
#include "packages.h"
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

}

//Callbacks and broadcast connection structure
static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_broadcast_process, ev, data) {
    static struct etimer et;
    rimeaddr_t dest;

    pkg_hdr to_send;
    //This address will be used to send the token to the first node
    dest.u8[0] = 1;
    dest.u8[1] = 0;


    //Adjacency matrix to be sent
    uchar adj[TOT_NUM_NODES * TOT_NUM_NODES];
    //Filling it
    memset(adj, 0, TOT_NUM_NODES * TOT_NUM_NODES);

    adj[mat2vec(0, 1)] = 1;

    adj[mat2vec(1, 2)] = 1;

    adj[mat2vec(2, 0)] = 1;

    adj[mat2vec(1, 0)] = 1;

    adj[mat2vec(2, 1)] = 1;

    adj[mat2vec(0, 2)] = 1;
    //---

    //Setting handlers and begin
    PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

    PROCESS_BEGIN();

    //Send start pkg in broadcast to all the agents
    broadcast_open(&broadcast, 129, &broadcast_call);
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    send_start_pkg_broad(&broadcast);

    //Send the adjacency matrix in broadcast to all the agents
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    send_adj_pkg_broad(&broadcast, adj);
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    //Send the token to the first agent
    to_send.type = TOKEN_PKG;
    to_send.data_len = 0;
    to_send.receiver = dest;
    packetbuf_clear();
    packetbuf_copyfrom(&to_send, sizeof (pkg_hdr));
    broadcast_send(&broadcast);

    //  while(1) {
    //    etimer_set(&et, CLOCK_SECOND);
    //    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    //    send_adj_pkg_broad(&broadcast,n,adj);
    //  }

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
