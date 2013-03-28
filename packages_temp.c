#include "packages.h"


void send_start_pkg_broad(struct broadcast_conn *broadcast)
{
   rimeaddr_t dest;
   dest.u8[0]=255;
   dest.u8[1]=255;
   pkg_hdr to_send;
   to_send.type=START_PKG;
   to_send.receiver=dest;
   to_send.data_len=0;
   packetbuf_clear();
   packetbuf_copyfrom(&to_send,sizeof(pkg_hdr));
   broadcast_send(broadcast); 
   
}

void send_stop_pkg_broad(struct broadcast_conn *broadcast)
{
   rimeaddr_t dest;
   dest.u8[0]=255;
   dest.u8[1]=255;
   pkg_hdr to_send;
   to_send.type=STOP_PKG;
   to_send.receiver=dest;
   to_send.data_len=0;
   packetbuf_clear();
   packetbuf_copyfrom(&to_send,sizeof(pkg_hdr));
   broadcast_send(broadcast); 
   
}

void send_adj_pkg_broad(struct broadcast_conn *broadcast, uchar n,uchar *adj)
{
   pkg_hdr to_send;
   rimeaddr_t dest;
   dest.u8[0]=255;
   dest.u8[1]=255;
   uchar pkg_length=sizeof(pkg_hdr)+n*n;
   uchar *buffer_to_send=malloc(pkg_length);
   to_send.type=ADJ_MATR_PKG;
   to_send.receiver=dest;
   to_send.data_len=n*n;
   memcpy(buffer_to_send,&to_send,sizeof(pkg_hdr));
   memcpy(buffer_to_send+sizeof(pkg_hdr),adj,n*n);
   packetbuf_clear();
   packetbuf_copyfrom(buffer_to_send,pkg_length);
   broadcast_send(broadcast); 
   free(buffer_to_send);
}

