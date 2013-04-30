/* 
 * File:   incident_edgeset.c
 * Author: user
 *
 * Created on April 26, 2013, 4:38 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include "incident_edgeset.h"

void init_incident_es(incident_edgeset* es)
{
    es->num_incident=0;
}

void add_edge_incident_es(incident_edgeset* es, edge to_add)
{
    es->incident_edges[es->num_incident].node_i=to_add.node_i;
    es->incident_edges[es->num_incident].node_j=to_add.node_j;
    es->num_incident++;
}

//Not visible function. This is an auxiliary function to shift back the edges by a 
//position starting from position "i"

void fix_remaining(incident_edgeset* es,uchar i)
{
    uchar j;
    
    for(j=i+1;j<es->num_incident;j++)
    {
        es->incident_edges[j-1].node_i=es->incident_edges[j].node_i;
        es->incident_edges[j-1].node_j=es->incident_edges[j].node_j;
    }
}

void remove_edge_incident_es(incident_edgeset* es, edge to_remove)
{
    uchar i;
    uchar stop=0;
    for(i=0;i<(es->num_incident-1) && !stop;i++)
    {
            
        if((es->incident_edges[i].node_i==to_remove.node_i) && (es->incident_edges[i].node_j==to_remove.node_j))
        {
            stop=1;
            //Avoid the last element
            //All the edges must be shifted back by a position
            fix_remaining(es,i);
            es->num_incident--;        
        }
    }
    if(es->incident_edges[es->num_incident-1].node_i==to_remove.node_i && es->incident_edges[es->num_incident-1].node_j==to_remove.node_j)
      es->num_incident--;
}

uchar is_contained_incident_es(incident_edgeset* es, edge to_find)
{
    uchar i;
    uchar result=0;
    
    for(i=0;i<TOT_NUM_NODES && !result;i++)
    {
        if((es->incident_edges[i].node_i==to_find.node_i) && (es->incident_edges[i].node_j==to_find.node_j))
            result=1;
    }
    return result;
}
