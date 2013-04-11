/* 
 * File:   pebble_assign_set.h
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

    typedef struct {
        edge assign_edges[2];
        uchar num_assigned;
    } edgeset;

    void init_edge(edgeset *ed);

    void add_edge(edgeset *ed, edge ed_to_add);

    uchar remove_edge(edgeset *ed, edge ed_to_add);

    uchar remove_single_edge(edgeset *ed, edge ed_to_add);

    extern edgeset assign_pebble;

    void print_pebble_assign(edgeset *ed);
#ifdef	__cplusplus
}
#endif

#endif	/* PEBBLE_ASSIGN_SET_H */

