#include "independent_edgeset.h"


void init_independent_es(independent_edgeset* es)
{
    es->num_independent=0;
}

void add_edge_independent_es(independent_edgeset* es, edge to_add)
{
    es->independent_edges[es->num_independent].node_i=to_add.node_i;
    es->independent_edges[es->num_independent].node_j=to_add.node_j;
    es->num_independent++;
}

uchar is_contained_independent_es(independent_edgeset* es, edge to_find)
{
    uchar i;
    uchar result=0;
    
    for(i=0;i<TOT_NUM_NODES && !result;i++)
    {
        if((es->independent_edges[i].node_i==to_find.node_i) && (es->independent_edges[i].node_j==to_find.node_j))
            result=1;
    }
    return result;
}
