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
#include "contiki-lib.h"
#include "contiki-net.h"
#include "net/rime.h"
#include "dev/button-sensor.h"
#include "net/netstack.h"
#include "cfs/cfs.h"
#include "dev/leds.h"
#include "packages_comm.h"
#include "lib/random.h"
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

uchar choose_random_edge(uchar v1, uchar *adj) {
    uchar i, random_value, diff_zero = 0;
    uchar aux[TOT_NUM_NODES];

    for (i = 0; i < TOT_NUM_NODES; i++) {
        if (adj[mat2vec(v1, i)]) {
            aux[diff_zero] = i;
            diff_zero++;
        }
    }
    if (!diff_zero)
        return 0;
    else {
        random_value = (uchar) (random_rand() % diff_zero);
    }
    return aux[random_value];
}

void remove_random_edge(uchar *adj) {
    uchar j;
    uchar r = (uchar) (random_rand() % TOT_NUM_NODES);
    uchar found = 0;

    for (j = 0; j < TOT_NUM_NODES && !found; j++) {
        if (adj[mat2vec(r, j)]) {
            found = 1;
            adj[mat2vec(r, j)] = 1;
            adj[mat2vec(j, r)] = 1;
        }
    }

}

/**
 * %HENNEBURGGRAPH Randomly generates a rigid graph with Henneburg operations
 * @param adj Adjacency matrix
 */
void henneburgGraph(uchar *adj) {
    uchar i, op, v1, v2, v3;
    //Add first bar between first 2 nodes
    adj[mat2vec(0, 1)] = 1;
    adj[mat2vec(1, 0)] = 1;


    //Perform N-2 random Henneburg operations to generate graph
    for (i = 2; i < TOT_NUM_NODES; i++) {

        if (i > 2)
            op = (uchar) (random_rand() % 2) + 1;
        else
            op = 1;
        // Perform the 2-valent operation
        if (op == 1) {
            //Choose vertices
            v1 = (uchar) (random_rand() % i);
            v2 = (uchar) (random_rand() % i);
            while (v2 == v1) {
                v2 = (uchar) (random_rand() % i);
            }


            //% Add new edges 
            adj[mat2vec(i, v1)] = 1;
            adj[mat2vec(v1, i)] = 1;
            adj[mat2vec(i, v2)] = 1;
            adj[mat2vec(v2, i)] = 1;


        }

        // Perform the 3-valent operation
        if (op == 2) {

            // Choose an edge
            v1 = (uchar) (random_rand() % i);

            v2 = choose_random_edge(v1, adj);

            // Choose a third vertex
            v3 = (uchar) (random_rand() % i);
            while (v3 == v1 || v3 == v2) {
                v3 = (uchar) (random_rand() % i);
            }

            //Remove edge (v1,v2) and add edges (v1,i), (v2,i), (v3,i)
            adj[mat2vec(v1, v2)] = 0;
            adj[mat2vec(v2, v1)] = 0;

            adj[mat2vec(v1, i)] = 1;
            adj[mat2vec(i, v1)] = 1;

            adj[mat2vec(v2, i)] = 1;
            adj[mat2vec(i, v2)] = 1;

            adj[mat2vec(v3, i)] = 1;
            adj[mat2vec(i, v3)] = 1;

        }


    }


}
static uchar temp_adj_matrix[TOT_NUM_NODES - 1][TOT_NUM_NODES - 1] = {
{0, 1, 0, 1, 0, 0, 1 },
{1, 0, 1, 0, 1, 0, 0 },
{0, 1, 0, 0, 1, 1, 0 },
{1, 0, 0, 0, 0, 1, 1 },
{0, 1, 1, 0, 0, 0, 1 },
{0, 0, 1, 1, 0, 0, 0 },
{1, 0, 0, 1, 1, 0, 0 }
};

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
    int q,fd;
    unsigned int new_seed;
    unsigned short op;
    //Adjacency matrix to be sent
    uchar adj[TOT_NUM_NODES * TOT_NUM_NODES];
    //Filling it
    uchar i, j;

    //Setting handlers and begin
    PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

    PROCESS_BEGIN();
    
    
    memset(adj, 0, TOT_NUM_NODES * TOT_NUM_NODES);
    set_adj_matrix(adj);
    
    /*henneburgGraph(adj);
    srand(new_seed);
    random_init(new_seed);
    
    op=(unsigned short) (random_rand() % 2) + 1;
    
    
    if (op==1) {
        for (q = 0; q < 3; q++) {
            remove_random_edge(adj);
        }
    }
    
    
    printf("[ ");
    for (i = 0; i < TOT_NUM_NODES; i++) {
        for (j = 0; j < TOT_NUM_NODES; j++) {
            printf("%d,\t", adj[mat2vec(i, j)]);
        }
        printf("\n");
    }
    printf("]\n");
*/


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
    NETSTACK_MAC.off(0);

    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
