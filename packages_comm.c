#include "packages_comm.h"

//Actual list of nodes addresses
rimeaddr_t nodes_addr_list[TOT_NUM_NODES];


//TODO... all the cut and paste code must be moved in a auxiliary function
//Any suggestion is appreciated

void send_start_pkg_broad(struct broadcast_conn *broadcast) {
    
    //Header to be sent
    pkg_hdr to_send;
    //Setting the header, the receiver and the length of the payload
    to_send.type = START_PKG;
    //Clearing the buffer for sending the package
    packetbuf_clear();
    packetbuf_set_datalen(sizeof (pkg_hdr));
    //Copying the data in the buffer
    packetbuf_copyfrom(&to_send, sizeof (pkg_hdr));
    //Broadcast send
    broadcast_send(broadcast);

}

void send_adj_pkg_broad(struct broadcast_conn *broadcast, uchar *adj) {
    pkg_hdr to_send;
    uint16 len = TOT_NUM_NODES*TOT_NUM_NODES;

    uint16 pkg_length = sizeof (pkg_hdr) + len;
    uchar buffer_to_send[pkg_length];

    to_send.type = ADJ_MATR_PKG;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));
    //Copying the adjacency matrix in the buffer with an offset of sizeof(pkg_hdr)
    memcpy(buffer_to_send + sizeof (pkg_hdr), adj, len);
    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(broadcast);
    
}


void send_leader_bid_pkg(struct broadcast_conn *broadcast, uchar id, uchar bid,uchar size_ind_set) {
    pkg_hdr to_send;
    uint16 len = sizeof (uchar)*3;

    uint16 pkg_length = sizeof (pkg_hdr) + len;
    uchar buffer_to_send[pkg_length];

    to_send.type = LEADER_BID_PKG;

    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));

    memcpy(buffer_to_send + sizeof (pkg_hdr), &id, sizeof (uchar));
    memcpy(buffer_to_send + sizeof (pkg_hdr) + sizeof (uchar), &bid, sizeof (uchar));
    memcpy(buffer_to_send + sizeof (pkg_hdr) + 2*sizeof (uchar), &size_ind_set, sizeof (uchar));
    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(broadcast);
    
}

void send_leader_election_pkg(struct broadcast_conn *broadcast) {
    pkg_hdr to_send;

    uint16 pkg_length = sizeof (pkg_hdr);

    to_send.type = LEADER_START_ELECTION_PKG;
    
    //Copying the header into the buffer
    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(&to_send, pkg_length);
    broadcast_send(broadcast);
}

void send_rigidity_pkg(struct broadcast_conn *broadcast, uchar rigidity) {
    pkg_hdr to_send;
    uint16 len = sizeof (uchar);

    uint16 pkg_length = sizeof (pkg_hdr) + len;
    uchar buffer_to_send[pkg_length];

    to_send.type = NOTIFY_RIGIDITY_PKG;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));

    memcpy(buffer_to_send + sizeof (pkg_hdr), &rigidity, len);
    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(broadcast);
    
}


void send_pebble_request_pkg(struct broadcast_conn *broadcast, uchar to, uchar from, uint16 uId) {
    pkg_hdr to_send;
    uint16 len = sizeof (uchar)*2 + sizeof (uint16);

    uint16 pkg_length = sizeof (pkg_hdr) + len;
    uchar buffer_to_send[pkg_length];

    
    to_send.type = REQUEST_PEBBLE_PKG;
    
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));
    memcpy(buffer_to_send + sizeof (pkg_hdr), &to, sizeof (uchar));
    memcpy(buffer_to_send + sizeof (pkg_hdr) + sizeof (uchar), &from, sizeof (uchar));
    memcpy(buffer_to_send + sizeof (pkg_hdr) + 2 * sizeof (uchar), &uId, sizeof (uint16));

    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(broadcast);
    
}

void send_back_pebble_pkg(struct broadcast_conn *broadcast, uchar to) {
    pkg_hdr to_send;
    uint16 len = sizeof (uchar)*2;

    uint16 pkg_length = sizeof (pkg_hdr) + len;
    uchar buffer_to_send[pkg_length];

    
    to_send.type = SEND_BACK_PEBBLE_PKG;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));

    memcpy(buffer_to_send + sizeof (pkg_hdr), &to, sizeof (uchar));
    memcpy(buffer_to_send + sizeof (pkg_hdr)+sizeof (uchar), &NODE_ID, sizeof (uchar));
    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(broadcast);
    
}

void send_take_back_pebbles(struct broadcast_conn *broadcast, uchar to, uchar from) {
    pkg_hdr to_send;
    uint16 len = 2 * sizeof (uchar);

    uint16 pkg_length = sizeof (pkg_hdr) + len;
    uchar buffer_to_send[pkg_length];

    to_send.type = TAKE_BACK_PEBBLES_PKG;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));
    memcpy(buffer_to_send + sizeof (pkg_hdr), &to, sizeof (uchar));
    memcpy(buffer_to_send + sizeof (pkg_hdr) + sizeof (uchar), &from, sizeof (uchar));

    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(broadcast);
    
}

void send_pebble_msg(struct broadcast_conn *broadcast, uchar to, uchar from, uchar found) {
    pkg_hdr to_send;
    uint16 len = 2 * sizeof (uchar);

    uint16 pkg_length = sizeof (pkg_hdr) + len;
    uchar buffer_to_send[pkg_length];
    
    switch (found) {
        case 1:
            to_send.type = PEBBLE_FOUND_PKG;
            break;
        case 0:
            to_send.type = PEBBLE_NOT_FOUND_PKG;
            break;
    }

    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));
    memcpy(buffer_to_send + sizeof (pkg_hdr), &to, sizeof (uchar));
    memcpy(buffer_to_send + sizeof (pkg_hdr) + sizeof (uchar), &from, sizeof (uchar));
    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(broadcast);
}

