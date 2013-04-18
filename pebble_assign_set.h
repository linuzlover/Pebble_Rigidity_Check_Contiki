/* 
 * File:   pebble_assign_set.h
 * This file contains the functions to manage the edgeset for the pebble assignment
 * Author: user
 *
 * Created on April 10, 2013, 1:02 PM
 */

#ifndef PEBBLE_ASSIGN_SET_H
#define	PEBBLE_ASSIGN_SET_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "pebble_globals.h"

    /**
     * \struct edgeset Struct representing the pebble assignment.
     */
    typedef struct {
        edge assign_edges[2];
        uchar num_assigned;
    } edgeset;

    /**
     * \var edgeset containing the pebble assignment
     */
    extern edgeset assign_pebble;
    /**
     * Init the assignment
     * @param ed Edgeset to init
     */
    void init_edge(edgeset *ed);
    /**
     * Add an edge to the assignment
     * @param ed Edgeset
     * @param ed_to_add edge to be added
     */
    void add_edge(edgeset *ed, edge ed_to_add);

    /**
     * Remove all the matching edges from the edgeset
     * @param ed Edgeset
     * @param ed_to_remove edge to be removed from the edgeset
     * @return the number of removed edges
     */
    uchar remove_edge(edgeset *ed, edge ed_to_remove);
    /**
     * Remove a single edge from the edgeset
     * @param ed Edgeset
     * @param ed_to_remove edge to be removed from the edgeset
     * @return 1 if the edge has been removed, 0 otherwise
     */
    uchar remove_single_edge(edgeset *ed, edge ed_to_remove);
    /**
     * Auxiliary function to print the edgeset assignment
     * @param ed
     */
    void print_pebble_assign(edgeset *ed);
#ifdef	__cplusplus
}
#endif

#endif	/* PEBBLE_ASSIGN_SET_H */

