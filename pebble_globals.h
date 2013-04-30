#ifndef _PEBBLE_GLOBALS_H_
#define _PEBBLE_GLOBALS_H_

//Debugging define. If deactivated, turns off all the printf
#define DEBUG
#ifdef DEBUG
#define PRINTD printf
#else
#define PRINTD(format, args...) ((void)0)
#endif
//----------------------------------------------------------

/**
 * This define scales the pauses inside the receiver.c. It must be a positive
 * integer.
 */
#define SCALE 2

/*Typedefs for 2 byte and single byte information*/
/*!\typedef  
 * Typedef for 16bit unsigned short.
 */
typedef unsigned short uint16;

/*!\typedef  
 * Typedef for 8bit unsigned char.
 */
typedef unsigned char uchar;

/**
 * !\struct edge An edge given by the global IDs of the two nodes
 */
typedef struct {
    uchar node_i;
    uchar node_j;
} edge;
/*! 
 *Define the total number of involved nodes. In future releases this will be a 
 *dynamic variable. \TODO: Change it to a variable instead of a define
 * TOT_NUM_NODES=(RECEIVERS+SENDER)=RECEIVERS+1
 */
#define TOT_NUM_NODES 8
/**
 * \var NODE ID or index of the node in global ordering
 */
extern uchar NODE_ID;
/**
 * \var is_leader Variable to represent the current leadership
 */
extern uchar IS_LEADER;
/**
 * \var been_leader Variable to represent the past leadership
 */
extern uchar BEEN_LEADER;
/**
 * \var requester
 */
extern uchar REQUESTER;
/**
 * \var paths_searched
 */
extern uchar PATHS_SEARCHED;
/**
 * \var request_id unique identifier of the packet
 */
extern uint16 REQUEST_ID;
/**
 * \var pebbles for the pebble game
 */
extern uchar PEBBLES;
/**
 * \var peb_assign assignment of the pebbles
 */
extern edge peb_assign[2];
/**
 * \var independent set
 */
extern edge ind_set[2 * TOT_NUM_NODES - 3];
/**
 * \var NUM_IND_SET size of the independent set
 */
extern uchar NUM_IND_SET;
/**
 * \var REQUEST_WAIT
 */
extern uchar REQUEST_WAIT;
/**
 * \var QUAD
 */
extern uchar QUAD;
/**
 * \var been_leader_tab Array containing 1 if an agent has been a leader
 */
extern uchar been_leader_tab[TOT_NUM_NODES];
/**
 * \var adj_matrix adjacency matrix
 */
extern uchar adj_matrix[TOT_NUM_NODES*TOT_NUM_NODES];
/**
 * \var incident_edges array of incident edges to the current agent
 */
extern edge incident_edges[TOT_NUM_NODES - 1];
/**
 * \var NUM_INCIDENT_EDGES number of incident edges
 */
extern uchar NUM_INCIDENT_EDGES;
/**
 * \var CONSIDERED_EDGE currently considered edge
 */
extern edge CONSIDERED_EDGE;
/**
 * \var MAX_ID ID of the maximum bidder
 */
extern uchar MAX_ID;
/**
 * \var MAX_BID BID of the maximum bidder
 */

extern uchar MAX_BID;
/**
 * \var NUM_NEIGHBORS number of neighbors of each agent.
 */

extern uchar NUM_NEIGHBORS;
/**
 * \var neighbors_list List of neighbors to each agent.
 */
extern uchar neighbors_list[TOT_NUM_NODES];

/**
 * \var IS_RIGID Variable representing the rigidity of the graph.
 */
extern uchar IS_RIGID;
/**
 * \var PREV_LEADER variable to keep track if in the previous algorithm loop the 
 * agent was the leader
 */
extern uchar PREV_LEADER;
/**
 * \var received_leader_bid bids received by the other agents
 */
extern uchar received_leader_bid[TOT_NUM_NODES];
/**
 * \var uId unique identified for the pebble request package
 */
extern uchar uId;

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
