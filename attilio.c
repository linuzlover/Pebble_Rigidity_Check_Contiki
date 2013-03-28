/*
 * Copyright (c) 2007, Swedish Institute of Computer Science.
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
 * This file is part of the Contiki operating system.
 *
 * $Id: example-broadcast.c,v 1.3 2010/11/06 15:03:48 adamdunkels Exp $
 */

/**
 * \file
 *         Testing the broadcast layer in Rime
 * \author
 *         Adam Dunkels <adam@sics.se>
 */

#include "contiki.h"
#include "net/rime.h"
#include "random.h"
#include "dev/button-sensor.h"
#include "dev/leds.h"
#include "packages.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>


static uchar START_FLAG=0;
static uchar NUM_NODES=0;
static uchar *adj_matrix=NULL;
static uchar MY_ID=255;
static uchar GOT_TOKEN=0;

rimeaddr_t nodes_addr_list[TOT_NUM_NODES];


static uchar get_id(rimeaddr_t *from)
{
	uchar i;
	
	for(i=0;i<TOT_NUM_NODES;i++)
	{
		if(rimeaddr_cmp(&nodes_addr_list[i],from))
			return i;
	}
	return 255;
}

static uchar is_directed_to_me(rimeaddr_t *from)
{
	if(adj_matrix==NULL)
		return 0;
	uchar from_id=get_id(from);
	if(adj_matrix[MY_ID*NUM_NODES+from_id]==1)
		return 1;
}
static void set_addr_list()
{
	rimeaddr_t temp;

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

}


/*---------------------------------------------------------------------------*/
PROCESS(example_broadcast_process, "Broadcast example");
AUTOSTART_PROCESSES(&example_broadcast_process);
/*---------------------------------------------------------------------------*/
static void
broadcast_recv(struct broadcast_conn *c, const rimeaddr_t *from)
{
  /*Header of the received packet*/
  pkg_hdr rec_hdr;
  /*Copy the broadcast buffer in the header structure*/
  memcpy(&rec_hdr,packetbuf_dataptr(),sizeof(pkg_hdr));
  uchar len=rec_hdr.data_len;
  /*Switch on the type of pkg*/
  switch(rec_hdr.type){
  /*Color Of the led to set*/
  uchar color;
  
		case CHANGE_LED:
		  /*Change the active LED*/
			memcpy(&color,packetbuf_dataptr()+sizeof(pkg_hdr),1);
			switch(color)
			{
				case 0:
					leds_off(LEDS_ALL);
					leds_toggle(LEDS_RED);
					break;
				case 1:
					leds_off(LEDS_ALL);
					leds_toggle(LEDS_YELLOW);
					break;
				case 2:
					leds_off(LEDS_ALL);
					leds_toggle(LEDS_GREEN);
					break;
				default:
					break;
			}
			break;
			/*PKG to start the algorithms*/
		case START_PKG:
		  /*Set flag and send an event*/
			START_FLAG=1;
			process_post(&example_broadcast_process,PROCESS_EVENT_MSG,NULL);
			//leds_toggle(LEDS_ALL);
			break;
			/*PKG to stop the algorithms*/
		case STOP_PKG:
		  	/*Set flag and send an event*/
		  	START_FLAG=0;
			process_post(&example_broadcast_process,PROCESS_EVENT_MSG,NULL);
			break;
		case ADJ_MATR_PKG:
			if(adj_matrix==NULL)
				adj_matrix=malloc(len);
			else
				{
					if(!(sqrt(rec_hdr.data_len)==NUM_NODES))
                    			{
						NUM_NODES= sqrt(rec_hdr.data_len);
						realloc(adj_matrix,len);
					}
				}
			memcpy(adj_matrix,packetbuf_dataptr()+sizeof(pkg_hdr),len);
			process_post(&example_broadcast_process,PROCESS_EVENT_MSG,NULL);
			//leds_toggle(LEDS_ALL);
			break;
		case TOKEN_PKG:
			if(rimeaddr_cmp(&rec_hdr.receiver,&rimeaddr_node_addr))
			{
				GOT_TOKEN=1;
				process_post(&example_broadcast_process,PROCESS_EVENT_MSG,NULL);
			}
			break;
		default:
			break;
	}

}

static const struct broadcast_callbacks broadcast_call = {broadcast_recv};
static struct broadcast_conn broadcast;
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_broadcast_process, ev, data)
{
   static struct etimer et;

  PROCESS_EXITHANDLER(broadcast_close(&broadcast);)

  PROCESS_BEGIN();

  MY_ID=get_id(&rimeaddr_node_addr);

  broadcast_open(&broadcast, 129, &broadcast_call);
  
  /*Start only when START_PKG has been received*/
	PROCESS_WAIT_EVENT_UNTIL(START_FLAG);
	PROCESS_WAIT_EVENT_UNTIL(adj_matrix!=NULL);
printf("%d\n",sizeof(adj_matrix));	
//int i,j;
//
//for(i=0;i<NUM_NODES;i++)
//{
//for(j=0;j<NUM_NODES;j++)
//	printf("%d\t",adj_matrix[i*NUM_NODES+j]);
//printf("\n");
//}
	/*Main loop*/
/*  while(1) {
	
    	PROCESS_WAIT_EVENT_UNTIL(GOT_TOKEN);
        leds_toggle(LEDS_ALL);        
        etimer_set(&et, 2*CLOCK_SECOND);
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        //send_token_pkg(struct broadcast_conn *broadcast, uchar n,uchar i,uchar *adj,rimeaddr_t nodes_addr_list[TOT_NUM_NODES])
	send_token_pkg(&broadcast, NUM_NODES,MY_ID,adj_matrix,nodes_addr_list);
        leds_toggle(LEDS_ALL);
    //printf("Alive\n");
  }
*/
  free(adj_matrix);
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
