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
Vector *vec_init_(int size, int type_size);
#define vec_init(size, type_size) vec_init_(size, sizeof(type_size))

/** Init the Vec from an array.
*   Copy the array into the Vec.
*   
*   @param arr          array
*   @param n            number of points 
*   @param type_size    sizeof(type)
* 
*   @return A Set of points
*/
Vector *vec_init_from_array_(void *arr, int n, int type_size);
#define vec_init_from_array(arr, n, type_size) vec_init_from_array_(arr, n, sizeof(type_size))

/** Destroy the Vec */
void vec_destroy(Vector *v);

/** Search the element equal to x*/
int vec_find(Vector *v, void *x, int (*cmp)(void*, void*));

/** Push x into the Vec */
void vec_push(Vector *v, void *x);

/** Get the elemente v[pos] */
void *vec_get(Vector *v, int pos);
#define vec_get_t(v, pos, type) *(type*)vec_get(v, pos)

/** Removes the element x[i]
*   It searches for the point on the Set
*
*   @warning: this operation is slow, use only 
*            to delete some elements, not all
*/
void vec_remove(Vector *v, int pos);

/** Removes x[i] and returns x[i]
*   
*   @warning this uses malloc
*/
void *vec_pop(Vector *v, int pos);
#define vec_pop_t(v, pos, Type) \
    ({ \
        Type *p = (Type*)vec_pop(v, pos); \
        Type  x = *p; \
        free(p); \
        x; \
    })

/** Returns the number of points on the Set */
int vec_len(Vector *v);

/** Get the pointer to the array of points 
*   warning: do not modify the array
*    
*   @return a pointer to an array
*/
void const *vec_to_array(Vector *v);
#define vec_to_array_t(v, type) ((type const *)vec_to_array(v))


/** Copy the vector into an array
*    
*   @return pointer to array
*/
void *vec_copy_to_array(Vector *v);


/** Get the pointer to the array and destroy the vec
*    
*   @return a pointer to array
*/
void *vec_to_array_and_destroy(Vector *v);


#endif 