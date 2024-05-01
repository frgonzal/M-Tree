#ifndef VECTOR_H
#define VECTOR_H


typedef struct vector Vector;


/** Init the Vector
*   
*   @param size         The initial size available
*   @param type_size    The size of the type. ej: sizeof(int)
*   
*   @return A Set of points
*/
Vector *vec_init(int size);

/** Init the Vec from an array.
*   Copy the array into the Vec.
*   
*   @param arr          array
*   @param n            number of points 
*   @param type_size    sizeof(type)
* 
*   @return A Set of points
*/
Vector *vec_init_from_array(void *arr, int n);

/** Destroy the Vec */
void vec_destroy(Vector *v);

/** Push x into the Vec */
void vec_push(Vector *v, void *x);

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


#endif 