#include <stdio.h>
#include <stdlib.h>
#include "../headers/mtree.h"
#include "../headers/point.h"


/*
typedef struct linked_list{
    Point p;
    linked_list *next;
} LinkedList;


Point* to_array(LinkedList *LinkedList, int *size){
    *size = 0;

    for(LinkedList *node=linked_list; node!=NULL; node=node->next)
        *size++;

    Point* points = (Points*) malloc(size*sizeof(Point));

    LinkedList *node=linked_list;
    while(LinkedList *node=linked_list; node!=NULL; node=node->next){
        points[i] = node->p;
    }

    return points;
}
*/


void recur_search(MTree *mtree, Point q, double r, Point* points, int* pos);

/** Calcula el largo del array */
int search_size(MTree *mtree, Point q, double r, int *size){
    double dist_pq = dist(mtree->p, q);

    if(mtree->a == NULL && dist_pq <= r)
        (*size)++;

    if(mtree->a != NULL && dist_pq <= mtree->cr + r){
        for(int i=0; i<mtree->n; i++){
            recur_search(mtree, q, r, NULL, NULL);
        }
    }   
    return 0;
}


/** Busca todos los puntos que estan en la query y los agrega a un array*/
void recur_search(MTree *mtree, Point q, double r, Point* points, int* pos){
    double dist_pq = dist(mtree->p, q);

    if(mtree->a == NULL && dist_pq <= r)
        points[*pos] = mtree->p;

    if(mtree->a != NULL && dist_pq <= mtree->cr + r){
        for(int i=0; i<mtree->n; i++){
            recur_search(mtree, q, r, points, pos);
        }
    }
}


Point* search(MTree *mtree, Point q, double r, int *size_arr, int *n_querys){
    search_size(mtree, q, r, size_arr);
    Point *points = (Point*) malloc((*size_arr)*sizeof(Point));

    int pos = 0;
    recur_search(mtree, q, r, points, &pos);

    return points;
}


void destroy(MTree *mtree){

    return;
}