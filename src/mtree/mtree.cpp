#include <cstddef>
#include <stdlib.h>
#include <cmath>
#include <tuple>
#include <vector>
#include "../../headers/mtree.hpp"
#include "../../headers/point.hpp"
#include "vector"
#include "queue"


MTree::MTree(){}
MTree::MTree(Node *node) : root(node) {};
MTree::~MTree(){}


Node::Node(): h(0), entries(std::vector<Entry>()){};
Node::~Node(){
    for(Entry e : entries)
        delete e.a;
};


Entry::Entry(): p({0, 0}), cr(0), a(nullptr){};
Entry::Entry(const Point &p): p(p), cr(0), a(nullptr) {};


//MTree::MTree(Point point) : p(point), cr(0), h(0), a(std::vector<MTree>(0)){}

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
    int ios = 0;
    std::queue<Node*> queue;
    queue.push(root);

    while(!queue.empty()){
        Node *node = queue.front();
        queue.pop();

        if(node == nullptr)
            continue;

        if(node->h == 0){//hojas
            for(Entry e : node->entries){
                double d = dist(e.p, q);
                if(d <= r)
                    v.push_back(e.p);
            }
        }else{// revisar nodos hijos
            for(Entry &e : node->entries){
                double d2 = dist(e.p, q);
                if(d2 <= e.cr + r)
                    queue.push(e.a);
            }
        }

        ios++; 
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
