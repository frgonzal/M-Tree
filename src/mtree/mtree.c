#include <stdio.h>
#include <stdlib.h>
#include "../../headers/mtree.h"
#include "../../headers/point.h"
#include "../../headers/utils/vector.h"
#include "../../headers/utils/queue.h"


/** Search Query
*   BFS algorithm
*   
*   @param mtree  MTree
*   @param q      Search center
*   @param r      Search radius
*   @param v      Vector to save the result
*   
*   @return Query I/Os
*/
static int query(MTree *mtree, Point q, double r, Vector *v){
    int n = 0;
    Queue *queue = q_init();
    q_put(queue, mtree);

    while(!q_empty(queue)){
        MTree* mtree = q_get(queue);

        double d2 = dist2(mtree->p, q);
        if(d2 <= r*r)
            vec_push(v, &mtree->p);
        
        if(mtree->a != NULL && d2 <= (mtree->cr + r)*(mtree->cr + r)){
            for(int i=0; i<mtree->n; i++)
                q_put(queue, mtree->a+i);
        }
        n++;
    }

    q_destroy(queue);
    return n;
}


MTreeSearch mtree_search(MTree *mtree, Point q, double r){
    if (mtree == NULL) 
        return (MTreeSearch){NULL, 0, 0};

    Vector *v = vec_init(64, Point);
    int ios = query(mtree, q, r, v);

    int size = vec_len(v);
    Point *points = vec_to_array_and_destroy(v);

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