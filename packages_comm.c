#include "packages_comm.h"

rimeaddr_t nodes_addr_list[TOT_NUM_NODES];

uchar START_FLAG = 0;
uchar ADJ_FLAG = 0;
uchar GOT_TOKEN = 0;


//TODO... all the cut and paste code must be moved in a auxiliary function

void send_start_pkg_broad(struct broadcast_conn *broadcast) {
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
    broadcast_send(broadcast);

}

//See previous function for comments

void send_stop_pkg_broad(struct broadcast_conn *broadcast) {
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
    broadcast_send(broadcast);

}

void send_adj_pkg_broad(struct broadcast_conn *broadcast, uchar *adj) {
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
    broadcast_send(broadcast);
    free(buffer_to_send);
}

void send_token_pkg(struct broadcast_conn *broadcast, uchar i, uchar *adj, rimeaddr_t nodes_addr_list[TOT_NUM_NODES]) {
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
    broadcast_send(broadcast);
    //Debugging

    free(buffer_to_send);
}

void send_leader_bid_pkg(struct broadcast_conn *broadcast, uchar id, uchar bid) {
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
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(broadcast);
    free(buffer_to_send);
}

void send_leader_election_pkg(struct broadcast_conn *broadcast) {
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
    broadcast_send(broadcast);
}

void send_rigidity_pkg(struct broadcast_conn *broadcast, uchar rigidity) {
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
    broadcast_send(broadcast);
    free(buffer_to_send);
}

void send_pebble_request_pkg(struct broadcast_conn *broadcast, uchar to, uchar from, uint16 uId) {
    pkg_hdr to_send;
    rimeaddr_t dest;
    uint16 len = sizeof (uchar)*2 + sizeof (uint16);

    uint16 pkg_length = sizeof (pkg_hdr) + len;
    uchar buffer_to_send[pkg_length];

    //TO BE FIXED
    dest.u8[0] = 255;
    dest.u8[1] = 255;
    //-----------

    to_send.type = REQUEST_PEBBLE_PKG;
    //to_send.receiver = dest;
    to_send.data_len = len;
    //Copying the header into the buffer
    
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));
    memcpy(buffer_to_send + sizeof (pkg_hdr), &to, sizeof (uchar));
    memcpy(buffer_to_send + sizeof (pkg_hdr) + sizeof (uchar), &from, sizeof (uchar));
    memcpy(buffer_to_send + sizeof (pkg_hdr) + 2 * sizeof (uchar), &uId, sizeof (uint16));

    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(broadcast);
    free(buffer_to_send);
}

void send_back_pebble_pkg(struct broadcast_conn *broadcast, uchar to) {
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
    memcpy(buffer_to_send + sizeof (pkg_hdr)+sizeof (uchar), &NODE_ID, sizeof (uchar));
    packetbuf_clear();
    packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(broadcast);
    free(buffer_to_send);
}

void send_current_ind_set(struct broadcast_conn *broadcast, uchar how_many_edges) {
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
    broadcast_send(broadcast);
    free(buffer_to_send);
}

void send_take_back_pebbles(struct broadcast_conn *broadcast, uchar to, uchar from) {
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
    broadcast_send(broadcast);
    free(buffer_to_send);
}

void send_pebble_msg(struct broadcast_conn *broadcast, uchar to, uchar from, uchar found) {
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
    broadcast_send(broadcast);
    free(buffer_to_send);
}

