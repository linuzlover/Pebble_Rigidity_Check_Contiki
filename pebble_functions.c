#include <string.h>

#include "pebble_functions.h"

//Initialization i-th agent variables
/**
 * \var is_leader Is the current agent the leader
 */
uchar is_leader=0;
/**
 * \var been_leader Has the current agent been a leader
 */
uchar been_leader=0;
/**
 * \var requester
 */
uchar requester=0;
/**
 * \var paths_searched
 */
uchar paths_searched=0;
/**
 * \var request_id Unique id of the request
 */
uchar request_id=0;
/**
 * \var pebbles Number of pebbles
 */
uchar pebbles=2;

edge peb_assign[2];

edge ind_set[2*TOT_NUM_NODES-3];

uchar num_ind_set=0;

uchar request_wait=0;

uchar quad=1;

uchar been_leader_tab[TOT_NUM_NODES];

uchar NODE_ID=255;

/*
 * \var adj_matrix Array representing the adjacency matrix
 */
uchar adj_matrix[TOT_NUM_NODES*TOT_NUM_NODES];

edge incident_edges[TOT_NUM_NODES-1];

uchar num_incident_edges=0;

edge considered_edge;


void leader_init(){
    uint16 i=0;
    uchar count=0;
    is_leader=1;
    been_leader=1;
    request_wait=0;
    quad=1;
    
    //Init the incident edges set
    for(i=0;i<TOT_NUM_NODES;i++)
    {
        //If the agents are neighbors
        if(adj_matrix[mat2vec(NODE_ID,i)])
        {
            //Set the first endpoint
            incident_edges[count].node_i=NODE_ID;
            //Sent the second endpoint
            incident_edges[count].node_j=i;
            //Increment the counter of incident edges
            count++;
        }
    }
    //Keep track of the number of incident edges
    num_incident_edges=count;
    
    if(num_incident_edges>0)
    {
        considered_edge.node_i=incident_edges[0].node_i;
        considered_edge.node_j=incident_edges[0].node_j;
    }
}

void agent_init(){
    //Not the leader
    is_leader=0;
    //Never been a leader
    been_leader=0;
    //No leaders
    memset(been_leader_tab,0,sizeof(been_leader_tab));
    //Clearing the adjacency matrix
    memset(adj_matrix,0,sizeof(adj_matrix));
    //No requesters
    requester=0;
    //No paths searched
    paths_searched=0;
    //No unique id for the request
    request_id=0;
    //2 initially free pebbles assigned
    pebbles=2;
}
