#include <string.h>

#include "pebble_functions.h"
#include "pebble_assign_set.h"
#include "incident_edgeset.h"


/**
 * @see pebble_globals.h for comments
 */

assignment_edgeset assign_pebble;

uchar PREV_LEADER=0;

uchar IS_RIGID = 0;

uchar IS_LEADER = 0;

uchar BEEN_LEADER = 0;

uchar REQUESTER = 0;

uchar PATHS_SEARCHED = 0;

uint16 REQUEST_ID = 0;

uchar PEBBLES = 2;

edge ind_set[2 * (TOT_NUM_NODES) - 3];

uchar NUM_IND_SET = 0;

uchar REQUEST_WAIT = 0;

uchar QUAD = 1;

uchar been_leader_tab[TOT_NUM_NODES];

uchar NODE_ID = 255;

uchar uId;

uchar adj_matrix[TOT_NUM_NODES*TOT_NUM_NODES];

incident_edgeset incident_es;


uchar count_incident_edges = 0;

uchar received_leader_bid[TOT_NUM_NODES];

uchar MAX_ID = 0;

uchar MAX_BID = 0;

/**
 * Function to retrieve the global ID (used also as an array index). The scope of this
 * function is within the file. Auxiliary function.
 * @param from Rime address of the node used to retrieve the global ID
 * @return Return the global ID
 */
static uchar get_id(rimeaddr_t *from) {
    uchar i;

    for (i = 0; i < TOT_NUM_NODES; i++) {
        if (rimeaddr_cmp(&nodes_addr_list[i], from))
            return i;
    }
    return 255;
}

void leader_election_reset() {
    memset(received_leader_bid, 0, TOT_NUM_NODES * sizeof (uchar));
    //Each one simulates that its own bid has been received
    received_leader_bid[NODE_ID] = 1;
    MAX_BID = 0;
    MAX_ID = 0;
}

uchar check_all_bids_rec() {
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
    edge temp;
    //Setting the variables
    IS_LEADER = 1;
    BEEN_LEADER = 1;
    REQUEST_WAIT = 0;
    QUAD = 1;
    been_leader_tab[NODE_ID] = 1;

    //Init the incident edges set
    for (i = 0; i < TOT_NUM_NODES-1; i++) {
        
        //If the agents are neighbors
        if (adj_matrix[mat2vec(NODE_ID, i)] && !been_leader_tab[i]) {
            //Set the first endpoint
            temp.node_i=NODE_ID;
            temp.node_j=i;
            add_edge_incident_es(&incident_es,temp);
            PRINTD("Agent %d, neighbor:%d\n",NODE_ID,i);
        }
    }
    
    #ifdef DEBUG
    PRINTD("Agent %d num_inc_edges:%d\n",NODE_ID,incident_es.num_incident);
    PRINTD("Peb assign 0: %d,%d\n",assign_pebble.assign_edges[0].node_i,assign_pebble.assign_edges[0].node_j);
    PRINTD("Peb assign 1: %d,%d\n",assign_pebble.assign_edges[1].node_i,assign_pebble.assign_edges[1].node_j);
    PRINTD("Num pebbles %d Agent %d\n",PEBBLES,NODE_ID);
    #endif
}

void agent_init() {
//Get the global ID
    NODE_ID = get_id(&rimeaddr_node_addr);
    uId = NODE_ID * (TOT_NUM_NODES * TOT_NUM_NODES);
    //Not the leader
    IS_LEADER = 0;
    //Never been a leader
    BEEN_LEADER = 0;
    //No requesters
    REQUESTER = 0;
    //No paths searched
    PATHS_SEARCHED = 0;
    //No unique id for the request
    REQUEST_ID = 0;
    //2 initially free pebbles assigned
    PEBBLES = 2;
    //No leaders
    memset(been_leader_tab, 0, sizeof (been_leader_tab));
    //No auction pkgs received
    memset(received_leader_bid, 0, sizeof (been_leader_tab));


    //Clearing the assignment set
    init_edge(&assign_pebble);
    init_incident_es(&incident_es);
    //No ind edges
    NUM_IND_SET = 0;
}

//Returns 1 when completed

uchar leader_run(struct broadcast_conn *broadcast) {
    //If there is a pending request
    if (REQUEST_WAIT) {
        return 0;
    }

    //Inspecting all the incident edges
    while (count_incident_edges < incident_es.num_incident) {

        //Quadrupling
        while (QUAD <= 4) {
            //If the i-th agent has a free pebble
            if (PEBBLES > 0) {
                //Add edge to P_i
                add_edge(&assign_pebble,incident_es.incident_edges[count_incident_edges]);
		//Decrement the free pebbles
                PEBBLES--;
                //Next step of quadruplication
                QUAD++;

            } else {
		//Waiting for the answer
                REQUEST_WAIT = 1;
                //The searched path
                PATHS_SEARCHED = 1;
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
        PEBBLES = 2;

        //------------------------------

        //Send back a pebble to e_i(2)
        send_back_pebble_pkg(broadcast,incident_es.incident_edges[count_incident_edges].node_j);
        //Add edge to ind set
        ind_set[NUM_IND_SET] = incident_es.incident_edges[count_incident_edges];
        //increment the ind_set_size
        NUM_IND_SET++;

        //If there are 2N-3 in the independent set..the graph is rigid
        if (NUM_IND_SET == 2 * (TOT_NUM_NODES-1) - 3) {
            send_rigidity_pkg(broadcast, 1);
            IS_RIGID = 1;
            leds_on(LEDS_ALL);
            return 1;
        }
        //take the next edge
        count_incident_edges++;
        //Back
        QUAD = 1;

    }
    //All local edges checked: initiate the leadership auction sending the current size of the independent set

    return 1;
}

void manage_pebble_request(struct broadcast_conn *broadcast, uchar from, uint16 rUid) {
    

    //Already requested
    if (REQUEST_ID == rUid) {
        //Pebble not found msg
        PRINTD("FOUND A MATCH IN REQUEST ID BY %d. SENDING A PEBBLE NOT FOUND to %d\n",NODE_ID,from);
        send_pebble_msg(broadcast, from, NODE_ID, 0);
        return;
    }
    //Storing the requestid
    REQUEST_ID = rUid;

    //If there are some pebbles
    if (PEBBLES > 0) {
        //Assign one of them
        edge temp;
        temp.node_i=NODE_ID;
        temp.node_j=from;
        add_edge(&assign_pebble,temp);
        PEBBLES--;
        //Pebble found msg
        send_pebble_msg(broadcast, from, NODE_ID, 1);
    }//No pebbles left
    else {
        //Request the pebble to your neighbor
        PATHS_SEARCHED = 1;
        REQUESTER = from;
        send_pebble_request_pkg(broadcast, assign_pebble.assign_edges[0].node_j, NODE_ID, rUid);
    }
}

void manage_pebble_found(struct broadcast_conn *broadcast, uchar from) {

    edge temp;

    temp.node_i = NODE_ID;
    temp.node_j = from;

    //Remove the edge
    remove_single_edge(&assign_pebble, temp);
    PRINTD("In %d\n", NODE_ID);
    //print the assignment for debug
    print_pebble_assign(&assign_pebble);

    if (IS_LEADER) {
        add_edge(&assign_pebble, incident_es.incident_edges[count_incident_edges]);
        PRINTD("In pebble Found leader Num assigned %d\n", assign_pebble.num_assigned);
        QUAD++;
        REQUEST_WAIT = 0;
    } else {
        temp.node_i = NODE_ID;
        temp.node_j = REQUESTER;
        add_edge(&assign_pebble, temp);
        send_pebble_msg(broadcast, REQUESTER, NODE_ID, 1);
    }

}

void manage_pebble_not_found(struct broadcast_conn *broadcast, uchar from) {

    if (PATHS_SEARCHED < 2) {
        //Pebble request message to the other path
        PATHS_SEARCHED = 2;
        send_pebble_request_pkg(broadcast, assign_pebble.assign_edges[1].node_j, NODE_ID, uId);
        PRINTD("Looking on the second path: pebble request sent to %d by %d paths_searched %d\n", assign_pebble.assign_edges[1].node_j, NODE_ID, PATHS_SEARCHED);
        //Searched on the second path

    } else {
        if (IS_LEADER) {
            //Maintain the uId
            uId++;
            //Return pebbles to e_i
            uchar temp_res = remove_edge(&assign_pebble, incident_es.incident_edges[count_incident_edges]);
            PEBBLES += temp_res;
            send_take_back_pebbles(broadcast, incident_es.incident_edges[count_incident_edges].node_j, NODE_ID);
            //---------------------
            //Remove the edge
            count_incident_edges++;
            //Starting over with the quadruplication process
            QUAD = 1;
            //Not waiting for any request to be satisfied
            REQUEST_WAIT = 0;
        } else
            send_pebble_msg(broadcast, from, NODE_ID, 0);
    }
}

void manage_take_back_pebbles(uchar from) {

    edge temp;
    temp.node_i = NODE_ID;
    temp.node_j = from;

    uchar temp_res = remove_edge(&assign_pebble, temp);
    PEBBLES += temp_res;
}

void leader_close(struct broadcast_conn *broadcast) {
    //Not leader anymore
    IS_LEADER = 0;
    if (all_been_leader()) {
        //Send the rigidity pkg to all the agents
        send_rigidity_pkg(broadcast, IS_RIGID);
        //Turns the leds on according to the rigidity
        if (IS_RIGID)
            leds_on(LEDS_ALL);
        else
            leds_on(LEDS_BLUE);
    }
        //Else... keep going
    else
        PREV_LEADER = 1;
}

void manage_send_back_pebble(uchar from) {
    edge temp;
    temp.node_i = NODE_ID;
    temp.node_j = from;

    uchar res;
    res = remove_single_edge(&assign_pebble, temp);
    PEBBLES += res;
}
