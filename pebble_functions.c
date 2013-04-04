#include <string.h>

#include "pebble_functions.h"

static uchar uId = 0;
//Initialization i-th agent variables
/**
 * \var is_leader Is the current agent the leader
 */
uchar is_leader = 0;
/**
 * \var been_leader Has the current agent been a leader
 */
uchar been_leader = 0;
/**
 * \var requester
 */
uchar requester = 0;
/**
 * \var paths_searched
 */
uchar paths_searched = 0;
/**
 * \var request_id Unique id of the request
 */
uchar request_id = 0;
/**
 * \var pebbles Number of pebbles
 */
uchar pebbles = 2;
/*
 * Pebbles assignment
 */
edge peb_assign[2];
/**
 * Independent edges set
 */
edge ind_set[2 * TOT_NUM_NODES - 3];
/**
 * Number of independent edges
 */
uchar num_ind_set = 0;
/*
 *Wait the request to be fulfilled
 */
uchar request_wait = 0;
/*
 *Step of edge quadruplication
 */
uchar quad = 1;
/**
 * Array containing who has been a leader
 */
uchar been_leader_tab[TOT_NUM_NODES];
/**
 *Unique id of the node
 */
uchar NODE_ID = 255;

/*
 * \var adj_matrix Array representing the adjacency matrix
 */
uchar adj_matrix[TOT_NUM_NODES*TOT_NUM_NODES];

/**
 *List of incident edges
 */
edge incident_edges[TOT_NUM_NODES - 1];

/*
 * number of incident edges
 */
uchar num_incident_edges = 0;

uchar count_incident_edges = 0;

uchar received_leader_bid[TOT_NUM_NODES];

uchar max_id = 0;

uchar max_bid = 0;

uchar LEADER_INIT_EL = 0;

void leader_election_init() {
    memset(received_leader_bid, 0, TOT_NUM_NODES * sizeof (uchar));
    received_leader_bid[NODE_ID] = 1;
    max_bid = 0;
    max_id = 0;
}

uchar check_all_leader_pkgs_rec() {
    uchar i = 0;
    uchar considered = 0;

    for (i = 0; i < TOT_NUM_NODES; i++) {
        if (received_leader_bid[i])
            considered++;
    }

    return considered == TOT_NUM_NODES - 1;
}

uchar all_been_leader() {
    uchar i = 0;
    uchar considered = 0;

    for (i = 0; i < TOT_NUM_NODES; i++) {
        if (been_leader_tab[i])
            considered++;
    }

    return considered == TOT_NUM_NODES - 1;
}

void leader_init() {
    //INdex
    uint16 i = 0;
    //Counter
    uchar count = 0;
    //Setting the variables
    is_leader = 1;
    been_leader = 1;
    request_wait = 0;
    quad = 1;
    been_leader_tab[NODE_ID] = 1;


    //Init the incident edges set
    for (i = 0; i < TOT_NUM_NODES; i++) {
        //If the agents are neighbors
        if (adj_matrix[mat2vec(NODE_ID, i)]) {
            //Set the first endpoint
            incident_edges[count].node_i = NODE_ID;
            //Sent the second endpoint
            incident_edges[count].node_j = i;
            //Increment the counter of incident edges
            count++;
        }
    }
    //Keep track of the number of incident edges
    num_incident_edges = count;

}

void agent_init() {
    //Not the leader
    is_leader = 0;
    //Never been a leader
    been_leader = 0;
    //No leaders
    memset(been_leader_tab, 0, sizeof (been_leader_tab));
    //Clearing the adjacency matrix
    memset(adj_matrix, 0, sizeof (adj_matrix));
    //No requesters
    requester = 0;
    //No paths searched
    paths_searched = 0;
    //No unique id for the request
    request_id = 0;
    //2 initially free pebbles assigned
    pebbles = 2;
}

//Returns 1 when completed

uchar leader_run(struct broadcast_conn *broadcast) {
    //If there is a pending request
    if (request_wait)
        return 0;
    //Inspecting all the incident edges
    while (count_incident_edges < num_incident_edges) {
        //Quadrupling
        while (quad <= 4) {
            //If the i-th agent has a free pebble
            if (pebbles > 0) {
                //Add edge to P_i
                peb_assign[2 - pebbles] = incident_edges[count_incident_edges];
                //Decrement the free pebbles
                pebbles--;
                //Next step of quadruplication
                quad++;
            } else {
                //Request pebble message to P_i(1,2) with UID
                send_pebble_request_pkg(broadcast, peb_assign[0].node_j, uId);
                //Keep uId unique
                uId++;
                //Waiting for the answer
                request_wait = 1;
                //The searched path
                paths_searched = 1;
                //Not finished yet
                return 0;
            }
        }

        //Clearing the pebble assignment
        peb_assign[0].node_i = 255;
        peb_assign[0].node_j = 255;
        peb_assign[1].node_i = 255;
        peb_assign[1].node_j = 255;
        pebbles = 2;
        //------------------------------

        //Send back a pebble to e_i(2)
        send_back_pebble_pkg(broadcast, incident_edges[count_incident_edges].node_j);
        //Add edge to ind set
        ind_set[num_ind_set] = incident_edges[count_incident_edges];
        //increment the ind_set_size
        num_ind_set++;

        //If there are 2N-3 in the independent set..the graph is rigid
        if (num_ind_set == 2 * TOT_NUM_NODES - 3) {
            send_rigidity_pkg(broadcast, 1);
            return 1;
        }
        //take the next edge
        count_incident_edges++;
        //Back
        quad = 1;

    }
    //All local edges checked: initiate the leadership auction sending the current size of the independent set
    send_current_ind_set(broadcast, num_ind_set);
    return 1;
}

void manage_pebble_request(struct broadcast_conn *broadcast,uchar rUid,uchar rid)
{
    
    if(request_id==uId)
    {
        //Send pebble not found msg
        //return;
    }
    request_id=uId;
    
    if(pebbles>0)
    {
        peb_assign[2-pebbles].node_i=NODE_ID;
        peb_assign[2-pebbles].node_j=rid;
        pebbles--;
        //send found pebble msg
    }
    else
    {
        send_pebble_request_pkg(broadcast,peb_assign[0].node_j,uId);
        paths_searched=1;
        requester=rid;
    }
}

void leader_close() {
    //Not leader anymore
    is_leader = 0;
    //Init the auction
    LEADER_INIT_EL = 1;
}