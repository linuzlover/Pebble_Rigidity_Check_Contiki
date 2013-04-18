#include "pebble_assign_set.h"

void init_edge(edgeset *ed) {
    uchar i;
    //255 is the default value
    for (i = 0; i < 2; i++) {
        ed->assign_edges[i].node_i = 255;
        ed->assign_edges[i].node_j = 255;
    }
    ed->num_assigned = 0;
}

void print_pebble_assign(edgeset *ed) {
    PRINTD("Pebble assign 1: %d %d\n", ed->assign_edges[0].node_i, ed->assign_edges[0].node_j);
    PRINTD("Pebble assign 2: %d %d\n", ed->assign_edges[1].node_i, ed->assign_edges[1].node_j);
    PRINTD("Pebble assign num assigned %d\n", ed->num_assigned);

}

void add_edge(edgeset *ed, edge ed_to_add) {
    //If it is full, print an error message
    if (ed->num_assigned == 2) {
        PRINTD("Error in adding the edge\n");
    } else {
        ed->assign_edges[ed->num_assigned].node_i = ed_to_add.node_i;
        ed->assign_edges[ed->num_assigned].node_j = ed_to_add.node_j;
    }
    ed->num_assigned++;
}

uchar remove_single_edge(edgeset *ed, edge ed_to_add) {
    uchar res = 0;
    //Check the second assignment
    if (ed->assign_edges[1].node_i == ed_to_add.node_i && ed->assign_edges[1].node_j == ed_to_add.node_j) {
        ed->assign_edges[1].node_i = 255;
        ed->assign_edges[1].node_j = 255;
        ed->num_assigned--;
        res = 1;
    }        //Check the first assignment if the first fails
    else if (ed->assign_edges[0].node_i == ed_to_add.node_i && ed->assign_edges[0].node_j == ed_to_add.node_j) {
        ed->assign_edges[0].node_i = ed->assign_edges[1].node_i;
        ed->assign_edges[0].node_j = ed->assign_edges[1].node_j;
        ed->assign_edges[1].node_i = 255;
        ed->assign_edges[1].node_j = 255;
        ed->num_assigned--;
        res = 1;
    }
    return res;
}

uchar remove_edge(edgeset *ed, edge ed_to_add) {
    uchar res = 0;
    //Check the second assignment
    if (ed->assign_edges[1].node_i == ed_to_add.node_i && ed->assign_edges[1].node_j == ed_to_add.node_j) {
        ed->assign_edges[1].node_i = 255;
        ed->assign_edges[1].node_j = 255;
        ed->num_assigned--;
        res++;
    }
    //Check the first assignment
    if (ed->assign_edges[0].node_i == ed_to_add.node_i && ed->assign_edges[0].node_j == ed_to_add.node_j) {
        ed->assign_edges[0].node_i = ed->assign_edges[1].node_i;
        ed->assign_edges[0].node_j = ed->assign_edges[1].node_j;
        ed->assign_edges[1].node_i = 255;
        ed->assign_edges[1].node_j = 255;
        ed->num_assigned--;
        res++;
    }
    return res;
}
