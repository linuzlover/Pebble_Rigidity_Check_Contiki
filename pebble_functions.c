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
/*
 * Pebbles assignment
 */
edge peb_assign[2];
/**
 * Independent edges set
 */
edge ind_set[2*TOT_NUM_NODES-3];
/**
 * Number of independent edges
 */
uchar num_ind_set=0;
/*
 *Wait the request to be fulfilled
 */
uchar request_wait=0;
/*
 *Step of edge quadruplication
 */
uchar quad=1;
/**
 * Array containing who has been a leader
 */
uchar been_leader_tab[TOT_NUM_NODES];
/**
 *Unique id of the node
 */
uchar NODE_ID=255;

/*
 * \var adj_matrix Array representing the adjacency matrix
 */
uchar adj_matrix[TOT_NUM_NODES*TOT_NUM_NODES];

/**
 *List of incident edges
 */
edge incident_edges[TOT_NUM_NODES-1];

/*
 * number of incident edges
 */
uchar num_incident_edges=0;

/**
 * Currently considered edge by the leader
 */
edge considered_edge;

uchar received_leader_bid[TOT_NUM_NODES];

uchar max_id=0;

uchar max_bid=0;

void leader_election_init()
{
    memset(received_leader_bid,0,TOT_NUM_NODES*sizeof(uchar));
    received_leader_bid[NODE_ID]=1;
    max_bid=0;
    max_id=0;
}

uchar check_all_leader_pkgs_rec()
{
    uchar i=0;
    uchar considered=0;
    
    for(i=0;i<TOT_NUM_NODES;i++)
    {
        if(received_leader_bid[i])
            considered++;
    }
    
    return considered==TOT_NUM_NODES-1;
}

uchar all_been_leader()
{
    uchar i=0;
    uchar considered=0;
    
    for(i=0;i<TOT_NUM_NODES;i++)
    {
        if(been_leader_tab[i])
            considered++;
    }
    
    return considered==TOT_NUM_NODES-1;
}
void leader_init(){
    //INdex
    uint16 i=0;
    //Counter
    uchar count=0;
    //Setting the variables
    is_leader=1;
    been_leader=1;
    request_wait=0;
    quad=1;
    been_leader_tab[NODE_ID]=1;
    
    
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
    
    //Set the considered edge for the quadruplication
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


