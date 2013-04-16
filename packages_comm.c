#include "packages_comm.h"

rimeaddr_t nodes_addr_list[TOT_NUM_NODES];

//TODO... all the cut and paste code must be moved in a auxiliary function

void send_start_pkg_broad(struct broadcast_conn *c) {
    //Destination address
    rimeaddr_t dest;
    //Setting to not significant values
    dest.u8[0] = 255;
    dest.u8[1] = 255;

    //Header to be sent
    pkg_hdr to_send;
    //Setting the header, the receiver and the length of the payload
    to_send.type = START_PKG;
    //to_send.receiver = dest;
    to_send.data_len = 0;
    //Clearing the buffer for sending the package
    packetbuf_clear();
    packetbuf_set_datalen(sizeof (pkg_hdr));
    //Copying the data in the buffer
    packetbuf_copyfrom(&to_send, sizeof (pkg_hdr));
    //Broadcast send
    broadcast_send(c);

}

//See previous function for comments

void send_stop_pkg_broad(struct broadcast_conn *c) {
    rimeaddr_t dest;
    dest.u8[0] = 255;
    dest.u8[1] = 255;
    pkg_hdr to_send;
    to_send.type = STOP_PKG;
    //to_send.receiver = dest;
    to_send.data_len = 0;
    packetbuf_clear();
    packetbuf_set_datalen(sizeof (pkg_hdr));
    packetbuf_copyfrom(&to_send, sizeof (pkg_hdr));
    broadcast_send(c);

}

void send_adj_pkg_broad(struct broadcast_conn *c, uchar *adj) {
    pkg_hdr to_send;
    rimeaddr_t dest;
    uint16 len = TOT_NUM_NODES*TOT_NUM_NODES;

    uint16 pkg_length = sizeof (pkg_hdr) + len;
    uchar buffer_to_send[pkg_length];

    dest.u8[0] = 255;
    dest.u8[1] = 255;

    to_send.type = ADJ_MATR_PKG;
    //  to_send.receiver = dest;
    to_send.data_len = len;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));
    //Copying the adjacency matrix in the buffer with an offset of sizeof(pkg_hdr)
    memcpy(buffer_to_send + sizeof (pkg_hdr), adj, len);
    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(c);
    free(buffer_to_send);
}

void send_token_pkg(struct broadcast_conn *c, uchar i, uchar *adj, rimeaddr_t nodes_addr_list[TOT_NUM_NODES]) {
    pkg_hdr to_send;
    rimeaddr_t dest;
    uchar j;
    uchar pkg_length = sizeof (pkg_hdr);
    uchar *buffer_to_send = malloc(pkg_length);

    //Destination is set to the default value 255.255
    dest.u8[0] = 255;
    dest.u8[1] = 255;
    to_send.type = TOKEN_PKG;
    to_send.data_len = 0;

    //Send the token to the first element in the i-th row equal to 1.
    for (j = 0; j < TOT_NUM_NODES; j++) {
        if (adj[mat2vec(i, j)] == 1) {
            dest = nodes_addr_list[j];

        }
    }
    //to_send.receiver = dest;
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));
    packetbuf_clear();
    //packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(c);
    //Debugging

    free(buffer_to_send);
}

void send_leader_bid_pkg(struct broadcast_conn *c, uchar id, uchar bid) {
    pkg_hdr to_send;
    rimeaddr_t dest;
    uint16 len = sizeof (uchar)*2;

    uint16 pkg_length = sizeof (pkg_hdr) + len;
    uchar buffer_to_send[pkg_length];

    dest.u8[0] = 255;
    dest.u8[1] = 255;

    to_send.type = LEADER_BID_PKG;
    //to_send.receiver = dest;
    to_send.data_len = len;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));
    memcpy(buffer_to_send + sizeof (pkg_hdr), &id, sizeof (uchar));
    memcpy(buffer_to_send + sizeof (pkg_hdr) + sizeof (uchar), &bid, sizeof (uchar));
    packetbuf_clear();
    //packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(c);
    free(buffer_to_send);
}

void send_leader_election_pkg(struct broadcast_conn *c) {
    pkg_hdr to_send;
    rimeaddr_t dest;

    uint16 pkg_length = sizeof (pkg_hdr);

    dest.u8[0] = 255;
    dest.u8[1] = 255;

    to_send.type = LEADER_START_ELECTION_PKG;
    //to_send.receiver = dest;
    to_send.data_len = 0;
    //Copying the header into the buffer

    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(&to_send, pkg_length);
    broadcast_send(c);
}

void send_rigidity_pkg(struct broadcast_conn *c, uchar rigidity) {
    pkg_hdr to_send;
    rimeaddr_t dest;
    uint16 len = sizeof (uchar);

    uint16 pkg_length = sizeof (pkg_hdr) + len;
    uchar buffer_to_send[pkg_length];

    dest.u8[0] = 255;
    dest.u8[1] = 255;

    to_send.type = NOTIFY_RIGIDITY_PKG;
    //to_send.receiver = dest;
    to_send.data_len = len;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));

    memcpy(buffer_to_send + sizeof (pkg_hdr), &rigidity, len);
    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(c);
    free(buffer_to_send);
}

uchar send_pebble_request_pkg(struct runicast_conn *c, uchar to, uchar from, uint16 uId) {
    //Protothread structure
    struct pt pt;
    //Timer
    static struct etimer et;
    //Header to send
    pkg_hdr to_send;
    //size of the payload
    uint16 len = sizeof (uchar)*2 + sizeof (uint16);
    //Length of the whole packet
    uint16 pkg_length = sizeof (pkg_hdr) + len;
    //Buffer to store the new package to be sent
    uchar buffer_to_send[pkg_length];
    //Begin protothread
    PT_BEGIN(&pt);
    
    //Setting the type of the pkg
    to_send.type = REQUEST_PEBBLE_PKG;
    //Setting the length
    to_send.data_len = len;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));
    //Copying the destination id
    memcpy(buffer_to_send + sizeof (pkg_hdr), &to, sizeof (uchar));
    //Copying the sender id
    memcpy(buffer_to_send + sizeof (pkg_hdr) + sizeof (uchar), &from, sizeof (uchar));
    //Copying the unique request id
    memcpy(buffer_to_send + sizeof (pkg_hdr) + 2 * sizeof (uchar), &uId, sizeof (uint16));
    //Clear the buffer
    packetbuf_clear();
    //Set its length
    packetbuf_set_datalen(pkg_length);
    //Copy the packet in the outgoing buffer
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    //Wait until the channel is free
    while (runicast_is_transmitting(c)) {
        etimer_set(&et, CLOCK_SECOND);
        PT_WAIT_WHILE(&pt, etimer_expired(&et));
        PRINTD("Hangs in runicast_is_transmitting\n");
    }
    //------------------------------
    
    PRINTD("Send Pebble req pkg from% d to %d, Addresses:%d.%d,%d.%d\n", NODE_ID, to, nodes_addr_list[NODE_ID].u8[0], nodes_addr_list[NODE_ID].u8[1], nodes_addr_list[to].u8[0], nodes_addr_list[to].u8[1]);
    //Send the package
    runicast_send(c, &(nodes_addr_list[to]), MAX_RETRANSMISSIONS);
    PT_END(&pt);
    return 1;
}

void send_back_pebble_pkg(struct runicast_conn *c, uchar to) {
    pkg_hdr to_send;
    rimeaddr_t dest;
    uint16 len = sizeof (uchar)*2;
    uint16 pkg_length = sizeof (pkg_hdr) + len;
    uchar buffer_to_send[pkg_length];

    //TO BE FIXED
    dest.u8[0] = 255;
    dest.u8[1] = 255;
    //-----------

    to_send.type = SEND_BACK_PEBBLE_PKG;
    //to_send.receiver = dest;
    to_send.data_len = len;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));

    memcpy(buffer_to_send + sizeof (pkg_hdr), &to, sizeof (uchar));
    memcpy(buffer_to_send + sizeof (pkg_hdr) + sizeof (uchar), &NODE_ID, sizeof (uchar));
    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    while (runicast_is_transmitting(c)) {
        PRINTD("Hangs in runicast_is_transmitting\n");
        clock_wait(100);
    }
    PRINTD("Send Back Pebble pkg from% d to %d, Addresses:%d.%d,%d.%d\n", NODE_ID, to, nodes_addr_list[NODE_ID].u8[0], nodes_addr_list[NODE_ID].u8[1], nodes_addr_list[to].u8[0], nodes_addr_list[to].u8[1]);
    runicast_send(c, &(nodes_addr_list[to]), MAX_RETRANSMISSIONS);
}

void send_current_ind_set(struct broadcast_conn *c, uchar how_many_edges) {
    pkg_hdr to_send;
    rimeaddr_t dest;
    uint16 len = sizeof (uchar);

    uint16 pkg_length = sizeof (pkg_hdr) + len;

    uchar buffer_to_send[pkg_length];

    //TO BE FIXED
    dest.u8[0] = 255;
    dest.u8[1] = 255;
    //-----------

    to_send.type = IND_SET_PKG;
    //to_send.receiver = dest;
    to_send.data_len = len;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));
    memcpy(buffer_to_send + sizeof (pkg_hdr), &how_many_edges, sizeof (uchar));
    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(c);
}

void send_take_back_pebbles(struct runicast_conn *c, uchar to, uchar from) {
    pkg_hdr to_send;
    rimeaddr_t dest;
    uint16 len = 2 * sizeof (uchar);

    uint16 pkg_length = sizeof (pkg_hdr) + len;
    uchar buffer_to_send[pkg_length];

    //TO BE FIXED
    dest.u8[0] = 255;
    dest.u8[1] = 255;
    //-----------


    to_send.type = TAKE_BACK_PEBBLES_PKG;
    //to_send.receiver = dest;
    to_send.data_len = len;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));

    memcpy(buffer_to_send + sizeof (pkg_hdr), &to, sizeof (uchar));
    memcpy(buffer_to_send + sizeof (pkg_hdr) + sizeof (uchar), &from, sizeof (uchar));

    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    while (runicast_is_transmitting(c)) {
        PRINTD("Hangs in runicast_is_transmitting\n");
        clock_wait(100);
    }
    PRINTD("Send Take Back Pebbles pkg from% d to %d, Addresses:%d.%d,%d.%d\n", NODE_ID, to, nodes_addr_list[NODE_ID].u8[0], nodes_addr_list[NODE_ID].u8[1], nodes_addr_list[to].u8[0], nodes_addr_list[to].u8[1]);
    runicast_send(c, &(nodes_addr_list[to]), MAX_RETRANSMISSIONS);
}

void send_pebble_msg(struct runicast_conn *c, uchar to, uchar from, uchar found) {
    pkg_hdr to_send;
    rimeaddr_t dest;
    uint16 len = 2 * sizeof (uchar);

    uint16 pkg_length = sizeof (pkg_hdr) + len;
    uchar buffer_to_send[pkg_length];
    //TO BE FIXED
    dest.u8[0] = 255;
    dest.u8[1] = 255;
    //-----------

    switch (found) {
        case 1:
            to_send.type = PEBBLE_FOUND_PKG;
            break;
        case 0:
            to_send.type = PEBBLE_NOT_FOUND_PKG;
            break;
    }

    //to_send.receiver = dest;
    to_send.data_len = len;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));
    memcpy(buffer_to_send + sizeof (pkg_hdr), &to, sizeof (uchar));
    memcpy(buffer_to_send + sizeof (pkg_hdr) + sizeof (uchar), &from, sizeof (uchar));
    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    while (runicast_is_transmitting(c)) {
        PRINTD("Hangs in runicast_is_transmitting\n");
        clock_wait(100);
    }
    PRINTD("Send Pebble MSG from% d to %d, Addresses:%d.%d,%d.%d\n", NODE_ID, to, nodes_addr_list[NODE_ID].u8[0], nodes_addr_list[NODE_ID].u8[1], nodes_addr_list[to].u8[0], nodes_addr_list[to].u8[1]);
    runicast_send(c, &(nodes_addr_list[to]), MAX_RETRANSMISSIONS);
}

