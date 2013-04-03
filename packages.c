#include "packages.h"

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
    to_send.receiver = dest;
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
    to_send.receiver = dest;
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
    unsigned char buffer_to_send[pkg_length];

    dest.u8[0] = 255;
    dest.u8[1] = 255;

    to_send.type = ADJ_MATR_PKG;
    to_send.receiver = dest;
    to_send.data_len = len;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));
    //Copying the adjacency matrix in the buffer with an offset of sizeof(pkg_hdr)
    memcpy(buffer_to_send + sizeof (pkg_hdr), adj, len);
    packetbuf_clear();
    //packetbuf_set_datalen(pkg_length);
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
    to_send.receiver = dest;
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
    unsigned char buffer_to_send[pkg_length];

    dest.u8[0] = 255;
    dest.u8[1] = 255;

    to_send.type = LEADER_BID_PKG;
    to_send.receiver = dest;
    to_send.data_len = len;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));

    memcpy(buffer_to_send + sizeof (pkg_hdr), &id, sizeof (uchar));
    memcpy(buffer_to_send + sizeof (pkg_hdr) + sizeof (uchar), &bid, sizeof (uchar));
    packetbuf_clear();
    //packetbuf_set_datalen(pkg_length);
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
    to_send.receiver = dest;
    to_send.data_len = 0;
    //Copying the header into the buffer

    packetbuf_clear();
    //packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(&to_send, pkg_length);
    broadcast_send(broadcast);
}

void send_rigidity_pkg(struct broadcast_conn *broadcast, uchar rigidity) {
    pkg_hdr to_send;
    rimeaddr_t dest;
    uint16 len = sizeof (uchar);

    uint16 pkg_length = sizeof (pkg_hdr) + len;
    unsigned char buffer_to_send[pkg_length];

    dest.u8[0] = 255;
    dest.u8[1] = 255;

    to_send.type = NOTIFY_RIGIDITY_PKG;
    to_send.receiver = dest;
    to_send.data_len = len;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));

    memcpy(buffer_to_send + sizeof (pkg_hdr), &rigidity, len);
    packetbuf_clear();
    //packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(broadcast);
    free(buffer_to_send);
}

void send_pebble_request_pkg(struct broadcast_conn *broadcast,uchar dId,uchar uId)
{
    pkg_hdr to_send;
    rimeaddr_t dest;
    uint16 len = sizeof (uchar)*2;

    uint16 pkg_length = sizeof (pkg_hdr) + len;
    unsigned char buffer_to_send[pkg_length];

    //TO BE FIXED
    dest.u8[0] = 255;
    dest.u8[1] = 255;
    //-----------
    
    to_send.type = REQUEST_PEBBLE_PKG;
    to_send.receiver = dest;
    to_send.data_len = len;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));

    memcpy(buffer_to_send + sizeof (pkg_hdr), &dId, sizeof(uchar));
    memcpy(buffer_to_send + sizeof (pkg_hdr)+sizeof(uchar), &uId, sizeof(uchar));
    
    packetbuf_clear();
    //packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(broadcast);
    free(buffer_to_send);
}

void send_back_pebble_pkg(struct broadcast_conn *broadcast,uchar dId)
{
    pkg_hdr to_send;
    rimeaddr_t dest;
    uint16 len = sizeof (uchar);

    uint16 pkg_length = sizeof (pkg_hdr) + len;
    unsigned char buffer_to_send[pkg_length];

    //TO BE FIXED
    dest.u8[0] = 255;
    dest.u8[1] = 255;
    //-----------
    
    to_send.type = SEND_BACK_PEBBLE_PKG;
    to_send.receiver = dest;
    to_send.data_len = len;
    //Copying the header into the buffer
    memcpy(buffer_to_send, &to_send, sizeof (pkg_hdr));

    memcpy(buffer_to_send + sizeof (pkg_hdr), &dId, sizeof(uchar));
    
    packetbuf_clear();
    //packetbuf_set_datalen(pkg_length);
    packetbuf_copyfrom(buffer_to_send, pkg_length);
    broadcast_send(broadcast);
    free(buffer_to_send);
}