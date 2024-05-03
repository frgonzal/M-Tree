#include <stdio.h>
#include <stdlib.h>
#include <tuple>
#include <vector>
#include "../../headers/mtree.hpp"
#include "../../headers/point.hpp"
#include "vector"
#include "queue"


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
static int query(MTree *mtree, Point q, double r, std::vector<Point> &v){
    int n = 0;
    std::queue<MTree*> queue;
    queue.push(mtree);

    while(!queue.empty()){
        MTree* mtree = queue.front();
        queue.pop();

        double d2 = dist2(mtree->p, q);
        if(d2 <= r*r)
            v.push_back(mtree->p);
        
        if(mtree->a != NULL && d2 <= (mtree->cr + r)*(mtree->cr + r)){
            for(int i=0; i<mtree->n; i++)
                queue.push(mtree->a+i);
        }
        n++;
    }

    return n;
}


std::tuple<std::vector<Point>,int> mtree_search(MTree *mtree, Point q, double r){
    std::vector<Point> v(0);
    if(mtree == nullptr)
        return std::make_tuple(v, 0);

    int ios = query(mtree, q, r, v);

    return std::make_tuple(v, ios);
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