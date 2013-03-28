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

void send_adj_pkg_broad(struct broadcast_conn *broadcast,uchar *adj)
{
   pkg_hdr to_send;
   rimeaddr_t dest;
   uint16 len=TOT_NUM_NODES*TOT_NUM_NODES;

   uint16 pkg_length=sizeof(pkg_hdr)+len;
   unsigned char buffer_to_send[pkg_length];   

   dest.u8[0]=255;
   dest.u8[1]=255;

   to_send.type=ADJ_MATR_PKG;
   to_send.receiver=dest;
   to_send.data_len=len;
   memcpy(buffer_to_send,&to_send,sizeof(pkg_hdr));
   memcpy(buffer_to_send+sizeof(pkg_hdr),adj,len);
   packetbuf_clear();
   packetbuf_copyfrom(buffer_to_send,pkg_length);
   broadcast_send(broadcast); 
   free(buffer_to_send);
}

void send_token_pkg(struct broadcast_conn *broadcast, uchar n,uchar i,uchar *adj,rimeaddr_t nodes_addr_list[TOT_NUM_NODES])
{
   pkg_hdr to_send;
   rimeaddr_t dest;
   uchar j;
   dest.u8[0]=255;
   dest.u8[1]=255;
   uchar pkg_length=sizeof(pkg_hdr);
   uchar *buffer_to_send=malloc(pkg_length);
   to_send.type=TOKEN_PKG;
   to_send.data_len=0;
   
   for(j=0;j<n;j++)
	{
		if(adj[i*TOT_NUM_NODES+j]==1)
			{
				dest=nodes_addr_list[j];
				
			}
	}
   to_send.receiver=dest;
   memcpy(buffer_to_send,&to_send,sizeof(pkg_hdr));
   packetbuf_clear();
   packetbuf_copyfrom(buffer_to_send,pkg_length);
   broadcast_send(broadcast); 
   printf("Sent to: %d.%d\n",dest.u8[0],dest.u8[1]);
   free(buffer_to_send);

}

uint16 mat2vec(uchar i,uchar j)
{
	uint16 res;
	res=i*TOT_NUM_NODES+j;
	return res;
}
