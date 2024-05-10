#include <stdlib.h>
#include <cmath>
#include <tuple>
#include <vector>
#include "../../headers/mtree.hpp"
#include "../../headers/point.hpp"
#include "vector"
#include "queue"


/** @struct MTree */
MTree::MTree(Node *node) : root(node) {};
MTree::~MTree(){
    delete root;
}

/** @struct Node */
Node::~Node(){
    for(Entry e : entries)
        delete e.a;
};

/** @struct Entry */
Entry::Entry(const Point &p): p(p), cr(0), a(nullptr) {};


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
static int query(Node *root, Point q, double r, std::vector<Point> &v){
    if(root == nullptr)
        return 0;

    int ios = 0;
    std::queue<Node*> queue;
    queue.push(root);


    while(!queue.empty()){
        Node *node = queue.front();
        queue.pop();

        ios++; 
        if(node->h == 0){// leaf
            for(Entry e : node->entries){
                double d2 = dist2(e.p, q);
                if(d2 <= r*r)
                    v.push_back(e.p);
            }
        }else{
            for(Entry e : node->entries){
                double d2 = dist2(e.p, q);
                double max_d2 = e.cr + r;
                if(d2 <= max_d2*max_d2 && e.a != nullptr)
                    queue.push(e.a);
            }
        }
    }
    return ios;
}


std::tuple<std::vector<Point>, int> mtree_search(MTree *mtree, Point q, double r){
    std::vector<Point> v(0);
    if(mtree == nullptr)
        return std::make_tuple(v, 0);

    int ios = query(mtree->root, q, r, v);
    return std::make_tuple(v, ios);
}
