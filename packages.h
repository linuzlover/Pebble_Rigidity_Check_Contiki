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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*Typedefs for 2 byte and single byte information*/
/*!\typedef  
 * Typedef for 16bit unsigned short.
 */
typedef unsigned short uint16;

/*!\typedef  
 * Typedef for 8bit unsigned char.
 */
typedef unsigned char uchar;


/*! 
 *Define the total number of involved nodes. In future releases this will be a 
 * dynamic variable. \TODO: Change it to a variable instead of a define
 */
#define TOT_NUM_NODES 4

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
    /**
     * \var receiver The rime address of the destination
     */
    rimeaddr_t receiver;
    /**
     *\var data_len Length of the data in the payload
     */
    uint16 data_len;
} pkg_hdr;

/**
 *\enum type Enum describing the type of package to be sent/received
 */
enum {
    //Pkg to start the algorithms
    START_PKG = 1,
    //Pkg to stop the algorithms
    STOP_PKG,
    //Pkg to send the desired ajacency matrix
    ADJ_MATR_PKG,
    //To be defined \TODO:
    RYAN_PKG,
    //Pkg to toggle the LEDS
    CHANGE_LED,
    //Pkg to model the TOKEN
    TOKEN_PKG,
    
    CONSENSUS_PKG
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


void send_consensus_pkg(struct broadcast_conn *broadcast,const float *ass_value);
/**
 * Inline function to get the index of the (i,j) element in the array from the matrix representation.
 * @param i I-th index
 * @param j J-th index
 * @return The value of the index in the array
 */
static inline uint16 mat2vec(uchar i, uchar j) {
    uint16 res;
    res = i * TOT_NUM_NODES + j;
    return res;
}
#endif
