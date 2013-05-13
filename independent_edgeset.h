/* 
 * File:   indipendent_edgeset.h
 * Author: user
 *
 * Created on May 13, 2013, 4:36 PM
 */

#ifndef INDEPENDENT_EDGESET_H
#define	INDEPENDENT_EDGESET_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdio.h>
#include <stdlib.h>
#include "pebble_globals.h"

    /**
     * \struct incident_edgeset Struct representing the incident edges.
     * This struct is intended to manage the incident edgeset of each agent.
     */
    typedef struct {
        //An array of edges.
        //The number of edges is upper bounded by the number of agents-1
        edge independent_edges[2*TOT_NUM_NODES-3];
        //Number of incident edges
        uchar num_independent;
    } independent_edgeset;

    //Extern declaration (to be actually declared in pebble functions where it is 
    //going to be used
    extern independent_edgeset independent_es;
    
    /**
     * Init the incident edgeset
     * @param es
     */
    void init_independent_es(independent_edgeset *es);
    /**
     * Add an edge to the edgeset
     * @param es Edgeset
     * @param to_add Edge to be added
     */
    void add_edge_independent_es(independent_edgeset *es,edge to_add);
    /**
     * Remove a single edge from the edgeset
     * @param es Edgeset
     * @param to_remove Edge to be removed
     */
    //void remove_edge_incident_es(incident_edgeset* es, edge to_remove);
    /**
     * This function search fo the edge to_find in the edgeset
     * @param es Edgeset
     * @param to_find Edge to find
     * @return 1 if the edge is contained, 0 otherwise
     */
    uchar is_contained_independent_es(independent_edgeset* es, edge to_find);



#ifdef	__cplusplus
}
#endif

#endif	/* INDIPENDENT_EDGESET_H */

