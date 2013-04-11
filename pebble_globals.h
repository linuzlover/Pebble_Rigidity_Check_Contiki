#ifndef _PEBBLE_GLOBALS_H_
#define _PEBBLE_GLOBALS_H_

#define DEBUG

#ifdef DEBUG
#define PRINTD printf
#else
#define PRINTD(format, args...) ((void)0)
#endif


#define MAX_RETRANSMISSIONS 4

/*Typedefs for 2 byte and single byte information*/
/*!\typedef  
 * Typedef for 16bit unsigned short.
 */
typedef unsigned short uint16;

/*!\typedef  
 * Typedef for 8bit unsigned char.
 */
typedef unsigned char uchar;

typedef struct{
    uchar node_i;
    uchar node_j;
}edge;


/*! 
 *Define the total number of involved nodes. In future releases this will be a 
 * dynamic variable. \TODO: Change it to a variable instead of a define
 */
#define TOT_NUM_NODES 8


extern uchar is_over;
/**
 * \var NODE Id or index of the node in global ordering
 */
extern uchar NODE_ID;
/**
 * \var is_leader Variable to represent the current leadership
 */
extern uchar is_leader;
/**
 * \var been_leader Variable to represent the past leadership
 */
extern uchar been_leader;

extern uchar requester;

extern uchar paths_searched;
/**
 * \var request_id unique identifier of the packet
 */
extern uint16 request_id;
/**
 * \var pebbles for the pebble game
 */
extern uchar pebbles;

extern edge peb_assign[2];

extern edge ind_set[2*TOT_NUM_NODES-3];

extern uchar num_ind_set;

extern uchar request_wait;

extern uchar quad;

extern uchar been_leader_tab[TOT_NUM_NODES];

extern uchar adj_matrix[TOT_NUM_NODES*TOT_NUM_NODES];

extern edge incident_edges[TOT_NUM_NODES-1];

extern uchar num_incident_edges;

extern edge considered_edge;

extern uchar max_id;

extern uchar max_bid;

extern uchar num_neighbors;

extern uchar neighbors_list[TOT_NUM_NODES];

extern uchar is_rigid;
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
