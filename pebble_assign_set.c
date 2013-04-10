#include "pebble_assign_set.h"

void init_edge(edgeset *ed)
{
    uchar i;
    for(i=0;i<2;i++)
    {
        ed->assign_edges[i].node_i=255;
        ed->assign_edges[i].node_j=255;
    }
    ed->num_assigned=0;
}

void add_edge(edgeset *ed,edge ed_to_add)
{
    if(ed->num_assigned==2)
    {
        PRINTD("Error in adding the edge\n");
    }
    else
    {
        ed->assign_edges[ed->num_assigned].node_i=ed_to_add.node_i;
        ed->assign_edges[ed->num_assigned].node_j=ed_to_add.node_j;
    }
    ed->num_assigned++;
}

uchar remove_edge(edgeset *ed,edge ed_to_add)
{
    uchar res=0;
    if(ed->assign_edges[1].node_i==ed_to_add.node_i && ed->assign_edges[1].node_j==ed_to_add.node_j)
    {
        ed->assign_edges[1].node_i=255;
        ed->assign_edges[1].node_j=255;
        ed->num_assigned--;
        res++;
    }
    if(ed->assign_edges[0].node_i==ed_to_add.node_i && ed->assign_edges[0].node_j==ed_to_add.node_j)
    {
        ed->assign_edges[0].node_i=ed->assign_edges[1].node_i;
        ed->assign_edges[0].node_j=ed->assign_edges[1].node_j;
        ed->assign_edges[1].node_i=255;
        ed->assign_edges[1].node_j=255;
        ed->num_assigned--;
        res++;
    }
    return res;
}
