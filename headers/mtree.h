#ifndef MTREE_H
#define MTREE_H

#include "point.h"

#define B 4096


/**
*   @struct MTree
*   
*   Representa un M-Tree
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
* 
*/
typedef struct mtree {
    Point  p;           // punto del nodo.
    double cr;          // radio del nodo.
    struct mtree *a;    // array de hijos del nodo.
    int n;              // numero de hijos del nodo.
} MTree;


/** Query de busqueda
*   Entrega todos los puntos contenidos en un area
*   Definida por un punto q y un radio r.
*   
*   @param mtree Puntero a un nodo del MTree
*   @param q Punto donde se hace la busqueda
*   @param r Radio de busqueda.
*   
*   @return Retorna un puntero a un array 
*           de puntos con el resultado
*/
Point* search(MTree *mtree, Point q, double r);
// opciones para el retorno de puntos
// 1- Lista enlazada: Hay que crear una estructura (facil y rapido)
// 2- Array: Se necesita calcular el tamaño de antemano
//           para no pedir varias veces memoria con malloc


/** Método Ciaccia-Patella
*   @param points array de puntos
*   @param n      tamaño del array
*/
MTree* cp_create(Point *points, int n);


/** Método Sexton-Swinbank 
*   @param points array de puntos
*   @param n      tamaño del array
*/
MTree* ss_create(Point *points, int n);


/** Destruye el MTree para liberar memoria 
*
*   @param mtree Nodo del mtree a destruir
*/
void destroy(MTree *mtree);


#endif