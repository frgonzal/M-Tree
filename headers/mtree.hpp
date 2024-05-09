#ifndef MTREE_HPP
#define MTREE_HPP

#include "point.hpp"
#include <vector>
#include <tuple>


struct Node;
struct Entry;
struct MTree;


/** Each entry on a Node contains
*   (p, cr, a).
*/
struct Entry{
    Point  p;
    double cr;
    Node   *a;// Pointer to the child Node

    /** Default constructor */
    Entry() = default;

    /** Constructor */
    Entry(const Point &p);

    // no need for destructor
};


/** A node contains entries */
struct Node{
    /** A vector with all the entries of the Node */
    std::vector<Entry> entries;

    /** The height of the Node, useful */
    int h;

    /** Constructor */
    Node() = default;
    /** Destructor */
    ~Node();
};


/** @struct MTree
*
*   An M-Tree is a tree composed of nodes that contain
*   entries (p, cr, a), where p is a point, cr is the 
*   covering radius (the maximum distance between p and
*   any point of the subtree related to its entry), and 
*   a is a disk address pointing to the page of its 
*   identified child node. If the node corresponds to a 
*   leaf, for simplicity, we will assume cr and a are 
*   null.
*/
struct MTree {
    /* The root is a pointer to the first Node */
    Node *root;

    /** Default Constructor*/
    MTree() = default;

    /** Constructor */
    MTree(Node *node);

    /** Destructor */
    ~MTree(); 
};


/** The max value of entries on a Node */
const int B = 4096 / sizeof(Entry);


/** Search Query
*   
*   @param mtree    The MTree
*   @param q        Center of the Query 
*   @param r        Search radius
*   
*   @return tuple {vector of points, number of IO's}
*/
std::tuple<std::vector<Point>, int> mtree_search(MTree *mtree, Point q, double r);


/** Ciaccia-Patella Method */
MTree* mtree_create_cp(const std::vector<Point> &points);


/** Sexton-Swinbank Method */
MTree* mtree_create_ss(const std::vector<Point> &points);


#endif