/* 
 * File:   incident_edgeset.h
 * Author: user
 *
 * Created on April 26, 2013, 4:29 PM
 */

#ifndef INCIDENT_EDGESET_H
#define	INCIDENT_EDGESET_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "pebble_globals.h"

    /**
     * \struct incident_edgeset Struct representing the incident edges.
     * This struct is intended to manage the incident edgeset of each agent.
     */
    typedef struct {
        //An array of edges.
        //The number of edges is upper bounded by the number of agents-1
        edge incident_edges[TOT_NUM_NODES-1];
        //Number of incident edges
        uchar num_incident;
    } incident_edgeset;

    //Extern declaration (to be actually declared in pebble functions where it is 
    //going to be used
    extern incident_edgeset incident_es;
    
    /**
     * Init the incident edgeset
     * @param es
     */
    void init_incident_es(incident_edgeset *es);
    /**
     * Add an edge to the edgeset
     * @param es Edgeset
     * @param to_add Edge to be added
     */
    void add_edge_incident_es(incident_edgeset *es,edge to_add);
    /**
     * Remove a single edge from the edgeset
     * @param es Edgeset
     * @param to_remove Edge to be removed
     */
    void remove_edge_incident_es(incident_edgeset* es, edge to_remove);
    /**
     * This function search fo the edge to_find in the edgeset
     * @param es Edgeset
     * @param to_find Edge to find
     * @return 1 if the edge is contained, 0 otherwise
     */
    uchar is_contained_incident_es(incident_edgeset* es, edge to_find);
#ifdef	__cplusplus
}
#endif

#endif	/* INCIDENT_EDGESET_H */

