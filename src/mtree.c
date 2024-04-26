#include <stdio.h>
#include <stdlib.h>
#include "../headers/mtree.h"
#include "../headers/point.h"


/** Variable global para la posicion en la que se debe 
*   guardar el siguiente punto en la query
*/
static int pos;


/** Calcula el numero de puntos del resultado.
*   Util para pedir memoria para el resultado.
*
*   @param mtree  Puntero al mtree
*   @param q      Punto central del radio de busqueda
*   @param r      Radio de busqueda
*   @param points Array de puntos donde se guardaran los resultados
*   
*   @return el numero de lecturas que se hacen en disco
*/
static int query_size(MTree *mtree, Point q, double r){
    double dist_pq = dist(mtree->p, q);
    int n = 0;

    if(mtree->a == NULL && dist_pq <= r){
        return 1;
    }

    if(mtree->a != NULL && dist_pq <= mtree->cr + r){
        for(int i=0; i<mtree->n; i++)
            n += query_size((mtree->a)+i, q, r);
    }   
    return n;
}


/** Busca todos los puntos que estan en la query y los agrega a un array.
*   
*   @param mtree  Puntero al mtree
*   @param q      Punto central del radio de busqueda
*   @param r      Radio de busqueda
*   @param points Array de puntos donde se guardaran los resultados
*   
*   @return el numero de lecturas que se hacen en disco
*/
static int query(MTree *mtree, Point q, double r, Point* points){
    int n = 1;

    double dist_pq = dist(mtree->p, q);

    if(mtree->a == NULL && dist_pq <= r){
        points[pos] = mtree->p;
        pos++;
        return n;
    }

    if(mtree->a != NULL && dist_pq <= mtree->cr + r){
        for(int i=0; i<mtree->n; i++)
            n += query((mtree->a)+i, q, r, points);
    }

    return n;


Point* search(MTree *mtree, Point q, double r, int *size_arr, int *IOs){
    // calculo del largo del array
    *size_arr = query_size(mtree, q, r);
    Point *points = (Point*) malloc((*size_arr)*sizeof(Point));

    // guardar los valores
    pos = 0;
    *IOs = query(mtree, q, r, points);

    return points;
}


static void destroy(MTree *mtree){
    if(mtree->a != NULL){
        for(int i=0; i<mtree->n; i++)
            destroy((mtree->a)+i);

        free(mtree->a);
    }
}

void mtree_destroy(MTree **p_mtree){
    destroy(*p_mtree);
    free(*p_mtree);
    *p_mtree = NULL;
}