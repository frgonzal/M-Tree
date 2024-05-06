#include <stdlib.h>
#include <cmath>
#include <tuple>
#include <vector>
#include "../../headers/mtree.hpp"
#include "../../headers/point.hpp"
#include "vector"
#include "queue"


MTree::MTree(Point point) : p(point), cr(0), h(0), a(std::vector<MTree>(0)){}

MTree::MTree(){}

MTree::~MTree(){}

void MTree::add_child(MTree child){
    a.push_back(child);

    double r = dist(p, child.p) + child.cr;
    cr = cr > r ? cr : r;

    h  = h > child.h ? h : child.h + 1;
}


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

        if(mtree->h == 0 && d2 <= r*r)// leaf
            v.push_back(mtree->p);
        
        if(mtree->a.size() > 0 && d2 <= (mtree->cr + r)*(mtree->cr + r)){
            for(int i=0; i<mtree->a.size(); i++)
                queue.push(&(mtree->a)[i]);
        }
        n++;//for each node
    }

    return n;
}


std::tuple<std::vector<Point>, int> mtree_search(MTree *mtree, Point q, double r){
    std::vector<Point> v(0);
    if(mtree == nullptr)
        return std::make_tuple(v, 0);

    int ios = query(mtree, q, r, v);

    return std::make_tuple(v, ios);
}
