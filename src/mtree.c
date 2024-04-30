#include <stdio.h>
#include <stdlib.h>
#include "../headers/mtree.h"
#include "../headers/point.h"
#include "../headers/point_set.h"


/** Search Query
*   
*   @param mtree  MTree
*   @param q      Search center
*   @param r      Search radius
*   @param s      Result set of points
*   
*   @return Query I/Os
*/
static int query(MTree *mtree, Point q, double r, PSet *s){
    int n = 1;

    double dist_pq = dist(mtree->p, q);

    if(dist_pq <= r)
        pset_push(s, mtree->p);

    if(mtree->a != NULL && dist_pq <= mtree->cr + r){
        for(int i=0; i<mtree->n; i++)
            n += query(mtree->a+i, q, r, s);
    }

    return n;
}


MTreeSearch mtree_search(MTree *mtree, Point q, double r){
    if (mtree == NULL) 
        return (MTreeSearch){NULL, 0, 0};


    PSet *s = pset_init(128);
    int ios = query(mtree, q, r, s);

    int size = pset_len(s);
    Point *points = pset_to_array_and_destroy(s);

    return (MTreeSearch){points,size,ios};
}


/** Destroy a node */
static void destroy_node(MTree *mtree){
    if (mtree == NULL) return;

    if(mtree->a != NULL){
        for(int i=0; i<mtree->n; i++)
            destroy_node((mtree->a)+i);

        free(mtree->a);
    }
}

void mtree_destroy(MTree *mtree){
    destroy_node(mtree);
    free(mtree);
}