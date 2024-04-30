#ifndef MTREE_H
#define MTREE_H

#include "point.h"

#define B 4096


/**
*   @struct MTree
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
typedef struct mtree {
    Point  p;           // Point coordinates
    double cr;          // Point radius
    struct mtree *a;    // Node childs
    int n;         
} MTree;

/** 
*   @struct MTreeSearch
*
*   Contains an array of points of the result
*   and the IO's of the search.
*/
typedef struct {
    Point *points;      // Search result
    int result_size;    // Array size
    int ios;            // Input/Ouput's
} MTreeSearch;


/** Search Query
*   
*   @param mtree Puntero a un nodo del MTree
*   @param q Punto donde se hace la busqueda
*   @param r Radio de busqueda
*   
*   @return Returns a struct with the result
*               {points, size, IO's}
*/
MTreeSearch mtree_search(MTree *mtree, Point q, double r);


/** Método Ciaccia-Patella
*
*   @param points array de puntos
*   @param n      tamaño del array
*/
MTree* mtree_create_cp(Point *points, int n);


/** Método Sexton-Swinbank 
*
*   @param points array de puntos
*   @param n      tamaño del array
*/
MTree* mtree_create_ss(Point *points, int n);


/** Free MTree memory
*
*   @param mtree
*/
void mtree_destroy(MTree *mtree);


#endif