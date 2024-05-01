#include <stdlib.h>
#include <string.h>
#include "../../headers/utils/vector.h"

struct vector{
    void *elements;
    int size, max_size, type_size;
};


Vector *vec_init_(int size, int type_size){
    int max_size = 1;
    while(max_size < size)
        max_size <<= 1;

    Vector *v = malloc(sizeof(Vector));
    *v = (Vector){
        malloc(max_size*type_size),
        0,
        max_size,
        type_size
    };

    return v;
}


Vector *vec_init_from_array_(void *arr, int n, int type_size){
    Vector *v = vec_init_(n, type_size); 
    memcpy(v->elements, arr, n*type_size);
    v->size = n;
    return v;
}


void vec_destroy(Vector *v){
    free(v->elements);
    free(v);
}


void vec_push(Vector *v, void *x){
    if(v->size>=v->max_size){
        v->max_size <<= 1;
        v->elements = realloc(v->elements, v->max_size*v->type_size);
    }
    memcpy(v->elements+v->size*v->type_size, x, v->type_size);
    v->size++;
}

void *vec_get(Vector *v, int pos){
    return v->elements + pos*v->type_size;
}

int vec_find(Vector *v, void *x, int (*cmp)(void*, void*)){
    for(int i=0; i<v->size; i++){
        void *y = v->elements + i*v->type_size;
        if((*cmp)(x, y) == 0)
            return i;
    }
    return -1;
}


void vec_remove(Vector *v, int pos){
    if(pos < 0 || v->size <= pos)
        return;
    
    int i = pos*v->type_size;
    int j = (pos+1)*v->type_size;
    int n = (v->size - pos)*v->type_size;
    memmove(v->elements+i, v->elements+j, n);
    (v->size)--;

    if(v->size < v->max_size >> 1){
        v->max_size >>= 1;
        v->elements = realloc(v->elements, v->max_size*v->type_size);
    }
}

void *vec_pop(Vector *v, int pos){
    if(pos < 0 || v->size <= pos)
        return NULL;

    void *elem = malloc(v->type_size);
    void *src = vec_get(v, pos);
    memcpy(elem, src, v->type_size);

    vec_remove(v, pos);
    return elem;
}

int vec_len(Vector *v){
    return v->size;
}


void const *vec_to_array(Vector *v){
    if(vec_len(v) == 0) 
        return NULL;

    return v->elements;
}


void *vec_copy_to_array(Vector *v){
    if(vec_len(v) == 0) 
        return NULL;

    void *arr = malloc(v->size*v->type_size);
    memcpy(arr, v->elements, v->size*v->type_size);
    return arr;
}


void *vec_to_array_and_destroy(Vector *v){
    if(vec_len(v) == 0){
        vec_destroy(v);
        return NULL;
    }

    void *arr = realloc(v->elements, v->size*v->type_size);
    free(v);
    return arr;
}

