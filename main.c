/**
 * %HENNEBURGGRAPH Randomly generates a rigid graph with Henneburg operations
 * @param adj Adjacency matrix
 */
#include "pebble_globals.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

uchar choose_random_edge(uchar v1, uchar *adj) {
    uchar i, random_value, diff_zero = 0;
    uchar aux[TOT_NUM_NODES];

    for (i = 0; i < TOT_NUM_NODES; i++) {
        if (adj[mat2vec(v1, i)]) {
            aux[diff_zero] = i;
            diff_zero++;
        }
    }
    if (!diff_zero)
        return 0;
    else {
        random_value = (uchar) (rand() % diff_zero);
    }
    return aux[random_value];
}

void remove_random_edge(uchar *adj) {
    uchar j;
    uchar r = (uchar) (rand() % TOT_NUM_NODES);
    uchar found = 0;

    for (j = 0; j < TOT_NUM_NODES && !found; j++) {
        if (adj[mat2vec(r, j)]) {
            found = 1;
            adj[mat2vec(r, j)] = 1;
            adj[mat2vec(j, r)] = 1;
        }
    }

}

void main(int argc,char **argv) {
    uchar i,j, op, v1, v2, v3;
    uchar adj[TOT_NUM_NODES*TOT_NUM_NODES];
    FILE *temp=fopen("temp.txt","w");
    memset(adj, 0, TOT_NUM_NODES * TOT_NUM_NODES);
    //Add first bar between first 2 nodes
    adj[mat2vec(0, 1)] = 1;
    adj[mat2vec(1, 0)] = 1;

    printf("Used Random Seed: %d\n",(int)time(NULL));
    srand(time(NULL));
    //Perform N-2 random Henneburg operations to generate graph
    for (i = 2; i < TOT_NUM_NODES; i++) {

        if (i > 2)
            op = (uchar) (rand() % 2) + 1;
        else
            op = 1;
        // Perform the 2-valent operation
        if (op == 1) {
            //Choose vertices
            v1 = (uchar) (rand() % i);
            v2 = (uchar) (rand() % i);
            while (v2 == v1) {
                v2 = (uchar) (rand() % i);
            }


            //% Add new edges 
            adj[mat2vec(i, v1)] = 1;
            adj[mat2vec(v1, i)] = 1;
            adj[mat2vec(i, v2)] = 1;
            adj[mat2vec(v2, i)] = 1;


        }

        // Perform the 3-valent operation
        if (op == 2) {

            // Choose an edge
            v1 = (uchar) (rand() % i);

            v2 = choose_random_edge(v1, adj);

            // Choose a third vertex
            v3 = (uchar) (rand() % i);
            while (v3 == v1 || v3 == v2) {
                v3 = (uchar) (rand() % i);
            }

            //Remove edge (v1,v2) and add edges (v1,i), (v2,i), (v3,i)
            adj[mat2vec(v1, v2)] = 0;
            adj[mat2vec(v2, v1)] = 0;

            adj[mat2vec(v1, i)] = 1;
            adj[mat2vec(i, v1)] = 1;

            adj[mat2vec(v2, i)] = 1;
            adj[mat2vec(i, v2)] = 1;

            adj[mat2vec(v3, i)] = 1;
            adj[mat2vec(i, v3)] = 1;

        }


    }

    fprintf(temp,"%s\n","static uchar temp_adj_matrix[TOT_NUM_NODES - 1][TOT_NUM_NODES - 1] = {");
    for (i=0;i<TOT_NUM_NODES-1;i++)
    {
        fprintf(temp,"{");
        for(j=0;j<TOT_NUM_NODES-1;j++)
        {
            if(j<TOT_NUM_NODES-2)
            {
            
                fprintf(temp,"%d, ",adj[mat2vec(i,j)]);
            }
            else
            {
                fprintf(temp,"%d ",adj[mat2vec(i,j)]);
            }
        }
        if(i<TOT_NUM_NODES-2)
            fprintf(temp,"},\n",adj[mat2vec(i,j)]);
        else
            fprintf(temp,"}\n",adj[mat2vec(i,j)]);
    }
    fprintf(temp,"};\n");
}
