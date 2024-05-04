#ifndef MTREE_HPP
#define MTREE_HPP

#include "point.hpp"
#include <vector>
#include <tuple>

// B = 4096 / sizeof(MTree)
#define B 73

/** @class MTree
*   
*   Un M-Tree es un árbol que está compuesto de nodos 
*   que contiene entradas (p, cr, a), donde p es un 
*   punto, cr es el radio cobertor (covering radius)
*   de este subárbol (la máxima distancia que hay
*   entre p y cualquier punto del subárbol relacionado 
*   a su entrada) y a una dirección en disco a la página
*   de su hijo identificado por la entrada de su nodo 
*   interno. Si el nodo corresponde a una hoja, por 
*   simplicidad asumiremos cr y a nulos. 
*/
class MTree{
    public:
        Point p;
        double cr;
        std::vector<MTree> a;
        int h;

        MTree();
        MTree(Point point);
        ~MTree(); 
        void add_child(MTree child);
};


/** Search Query
*   
*   @param mtree Puntero a un nodo del MTree
*   @param q Punto donde se hace la busqueda
*   @param r Radio de busqueda
*   
*   @return Returns a struct with the result
*               {points, size, IO's}
*/
std::tuple<std::vector<Point>, int> mtree_search(MTree *mtree, Point q, double r);


/** Método Ciaccia-Patella
*
*   @param points array de puntos
*   @param n      tamaño del array
*/
MTree* mtree_create_cp(const std::vector<Point> &points);


/** Método Sexton-Swinbank 
*
*   @param points array de puntos
*   @param n      tamaño del array
*/
MTree* mtree_create_ss(const std::vector<Point> &points);

#endif