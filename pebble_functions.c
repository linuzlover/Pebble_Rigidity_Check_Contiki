#include <string.h>

#include "pebble_functions.h"
#include "pebble_assign_set.h"


edgeset assign_pebble;

uchar PREV_LEADER=0;

uchar is_rigid = 0;
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
uint16 request_id = 0;
/**
 * \var pebbles Number of pebbles
 */
uchar pebbles = 2;
/**
 * Independent edges set
 */
edge ind_set[2 * (TOT_NUM_NODES) - 3];
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

uchar uId;
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
uchar num_incident_edges = 0;

uchar count_incident_edges = 0;

uchar received_leader_bid[TOT_NUM_NODES];

uchar max_id = 0;

uchar max_bid = 0;


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
    for (i = 0; i < TOT_NUM_NODES-1; i++) {
        
        //If the agents are neighbors
        if (adj_matrix[mat2vec(NODE_ID, i)] && !been_leader_tab[i]) {
            //Set the first endpoint
            incident_edges[count].node_i = NODE_ID;
            //Sent the second endpoint
            incident_edges[count].node_j = i;
            //Increment the counter of incident edges
            count++;
            PRINTD("Agent %d, neighbor:%d\n",NODE_ID,i);
        }
    }
    //Keep track of the number of incident edges

    num_incident_edges = count;
    #ifdef DEBUG
    PRINTD("Agent %d num_inc_edges:%d\n",NODE_ID,num_incident_edges);
    PRINTD("Peb assign 0: %d,%d\n",assign_pebble.assign_edges[0].node_i,assign_pebble.assign_edges[0].node_j);
    PRINTD("Peb assign 1: %d,%d\n",assign_pebble.assign_edges[1].node_i,assign_pebble.assign_edges[1].node_j);
    PRINTD("Num pebbles %d Agent %d\n",pebbles,NODE_ID);
    #endif
}

void agent_init() {

    uId = NODE_ID * (TOT_NUM_NODES * TOT_NUM_NODES);
    //Not the leader
    is_leader = 0;
    //Never been a leader
    been_leader = 0;
    //No requesters
    requester = 0;
    //No paths searched
    paths_searched = 0;
    //No unique id for the request
    request_id = 0;
    //2 initially free pebbles assigned
    pebbles = 2;
    //No leaders
    memset(been_leader_tab, 0, sizeof (been_leader_tab));
    //No auction pkgs received
    memset(received_leader_bid, 0, sizeof (been_leader_tab));


    //Clearing the assignment set
    init_edge(&assign_pebble);

    //No ind edges
    num_ind_set = 0;
}

//Returns 1 when completed

uchar leader_run(struct broadcast_conn *broadcast) {
    //If there is a pending request
    if (request_wait) {
        return 0;
    }

    //Inspecting all the incident edges
    while (count_incident_edges < num_incident_edges) {

        //Quadrupling
        while (quad <= 4) {
            //If the i-th agent has a free pebble
            if (pebbles > 0) {
                //Add edge to P_i
                add_edge(&assign_pebble,incident_edges[count_incident_edges]);
		//Decrement the free pebbles
                pebbles--;
                //Next step of quadruplication
                quad++;

            } else {
		//Waiting for the answer
                request_wait = 1;
                //The searched path
                paths_searched = 1;
                //Request pebble message to P_i(1,2) with UID
                send_pebble_request_pkg(broadcast, assign_pebble.assign_edges[0].node_j, NODE_ID, uId);
                //Keep uId unique
                uId++;
                
                //Not finished yet
                return 0;
            }

        }
        //Clearing the pebble assignment
        init_edge(&assign_pebble);
        pebbles = 2;

        //------------------------------

        //Send back a pebble to e_i(2)
        send_back_pebble_pkg(broadcast, incident_edges[count_incident_edges].node_j);
        //Add edge to ind set
        ind_set[num_ind_set] = incident_edges[count_incident_edges];
        //increment the ind_set_size
        num_ind_set++;

        //If there are 2N-3 in the independent set..the graph is rigid
        if (num_ind_set == 2 * (TOT_NUM_NODES-1) - 3) {
            send_rigidity_pkg(broadcast, 1);
            is_rigid = 1;
            leds_on(LEDS_ALL);
            return 1;
        }
        //take the next edge
        count_incident_edges++;
        //Back
        quad = 1;

    }
    //All local edges checked: initiate the leadership auction sending the current size of the independent set

    return 1;
}

void manage_pebble_request(struct broadcast_conn *broadcast, uchar from, uint16 rUid) {
    

    //Already requested
    if (request_id == rUid) {
        //Pebble not found msg
        PRINTD("FOUND A MATCH IN REQUEST ID BY %d. SENDING A PEBBLE NOT FOUND to %d\n",NODE_ID,from);
        send_pebble_msg(broadcast, from, NODE_ID, 0);
        return;
    }
    //Storing the requestid
    request_id = rUid;

    //If there are some pebbles
    if (pebbles > 0) {
        //Assign one of them
        edge temp;
        temp.node_i=NODE_ID;
        temp.node_j=from;
        add_edge(&assign_pebble,temp);
        pebbles--;
        //Pebble found msg
        send_pebble_msg(broadcast, from, NODE_ID, 1);
    }//No pebbles left
    else {
        //Request the pebble to your neighbor
        paths_searched = 1;
	requester = from;
        send_pebble_request_pkg(broadcast, assign_pebble.assign_edges[0].node_j, NODE_ID, rUid);
    }
}

void manage_pebble_found(struct broadcast_conn *broadcast, uchar from) {
    
    edge temp;
    
    temp.node_i=NODE_ID;
    temp.node_j=from;
    
    remove_single_edge(&assign_pebble,temp);
    PRINTD("In %d\n",NODE_ID);
    print_pebble_assign(&assign_pebble);
        
    if (is_leader) {
        add_edge(&assign_pebble,incident_edges[count_incident_edges]);
	PRINTD("In pebble Found leader Num assigned %d\n",assign_pebble.num_assigned);
        quad++;
        request_wait = 0;
    } else {
        temp.node_i=NODE_ID;
        temp.node_j=requester;
        add_edge(&assign_pebble,temp);
        send_pebble_msg(broadcast, requester, NODE_ID, 1);
    }

}

void manage_pebble_not_found(struct broadcast_conn *broadcast, uchar from) {
    
    if (paths_searched < 2) {
        //Pebble request message to the other path
	paths_searched = 2;
	send_pebble_request_pkg(broadcast, assign_pebble.assign_edges[1].node_j, NODE_ID, uId);	
	PRINTD("Looking on the second path: pebble request sent to %d by %d paths_searched %d\n",assign_pebble.assign_edges[1].node_j,NODE_ID,paths_searched);
 	//Searched on the second path

    } else {
        if (is_leader) {
	    //Maintain the uId
            uId++;
            //Return pebbles to e_i
            uchar temp_res=remove_edge(&assign_pebble,incident_edges[count_incident_edges]);
            pebbles+=temp_res;
            send_take_back_pebbles(broadcast, incident_edges[count_incident_edges].node_j, NODE_ID);
            //---------------------
            //Remove the edge
            //incident_edges[count_incident_edges].node_i = 255;
            //incident_edges[count_incident_edges].node_j = 255;
            count_incident_edges++;
            //Starting over with the quadruplication process
            quad = 1;
            //Not waiting for any request to be satisfied
            request_wait = 0;
        } else
            send_pebble_msg(broadcast, from, NODE_ID, 0);
    }
}

void manage_take_back_pebbles(uchar from) {
    
    edge temp;
    temp.node_i=NODE_ID;
    temp.node_j=from;
    
    uchar temp_res=remove_edge(&assign_pebble,temp);
    pebbles+=temp_res;
}

void leader_close(struct broadcast_conn *broadcast) {
    //Not leader anymore
    is_leader = 0;
    send_current_ind_set(broadcast, num_ind_set);
    if (all_been_leader()) {
        send_rigidity_pkg(broadcast, is_rigid);
    } 
    else
        PREV_LEADER=1;
}

void manage_send_back_pebble(uchar from)
{
    edge temp;
    temp.node_i=NODE_ID;
    temp.node_j=from;
 
    uchar res;
    res=remove_single_edge(&assign_pebble,temp);
    pebbles+=res;      
}
