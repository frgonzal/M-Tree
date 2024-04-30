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
*   
*   @return The number of points that are contained in the search
*/
static int query_size(MTree *mtree, Point q, double r){
    double dist_pq = dist(mtree->p, q);
    int n = 0;

    if(dist_pq <= r) 
        n++;

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

    if(dist_pq <= r)
        points[pos++] = mtree->p;

    if(mtree->a != NULL && dist_pq <= mtree->cr + r){
        for(int i=0; i<mtree->n; i++)
            n += query((mtree->a)+i, q, r, points);
    }

    return n;
}


MTreeSearch mtree_search(MTree *mtree, Point q, double r){
    if (mtree == NULL) 
        return (MTreeSearch){NULL, 0, 0};

    int result_size = query_size(mtree, q, r);
    Point *points = (Point*) malloc(result_size*sizeof(Point));

    pos = 0;
    int ios = query(mtree, q, r, points);

    return (MTreeSearch){points,result_size,ios};
}


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
    *mtree = (MTree){0, 0, 0, NULL, 0};
}