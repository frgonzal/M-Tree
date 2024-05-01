#include <stdlib.h>
#include <string.h>
#include "../headers/vector.h"

struct vector{
    void *elements;
    int size, max_size;
};


Vector *vec_init(int size){
    int max_size = 1;
    while(max_size < size)
        max_size <<= 1;

    Vector *v = malloc(sizeof(Vector));
    *v = (Vector){
        malloc(max_size*sizeof(void*)),
        0,
        max_size
    };

    return v;
}


/** Init the Vec from an array.
*   Copy the array into the Vec.
*   
*   @param points   array of points
*   @param n        number of points 
* 
*   @return A Set of points
*/
Vector *vec_init_from_array(void *arr, int n){
    Vector *v = vec_init(n); 
    //    memcpy(v->elements, arr, n*sizeof(void*));
    return v;
}


/** Destroy the Vec */
void vec_destroy(Vector *v){
    free(v->elements);
    free(v);
}

/** Push x into the Vec */
void vec_push(Vector *v, void *x){
    if(v->size>=v->max_size){
        v->max_size <<= 1;
        v->elements = realloc(v->elements, v->max_size*sizeof(void*));
    }
    //memcpy
    //(v->elements)[v->size] = x;
}

/** Removes the element x[i]
*   It searches for the point on the Set
*/
void vec_remove(Vector *v, void *x);

/** Returns the number of points on the Set */
int vec_len(Vector *v);

/** Get the pointer to the array of points 
*   warning: do not modify the array
*    
*   @return a pointer to an array of points
*/
void *pset_to_array(Vector *s);

/** Get the pointer to the array of points
*   and then destroy the Set.
*
*   Could be useful if only want the array
*/
void *pset_to_array_and_destroy(Vector *s);