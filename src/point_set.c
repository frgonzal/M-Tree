#include <stdlib.h>
#include <string.h>
#include "../headers/point_set.h"
#include "../headers/point.h"


struct point_set {
    Point *points;
    int size; 
    int max_size;
};


PSet *pset_init(int size){
    int max_size = 0x1;
    while(size >= max_size)
        max_size <<= 1;

    Point *points = malloc(max_size*sizeof(Point));
    PSet *s = malloc(sizeof(PSet));

    *s = (PSet){points, 0, max_size};
    return s;
}


void pset_destroy(PSet *s){
    free(s->points);
    free(s);
}


void pset_push(PSet *s, Point p){
    if (s->size >= s->max_size){
        s->max_size <<= 1;
        s->points = realloc(s->points, s->max_size*sizeof(Point));
    }

    (s->points)[s->size++] = p;
}


static void remove_point(PSet *s, int i){
    for (int j=i; j<s->size-1; j++)
        (s->points)[j] = (s->points)[j+1];
}

void pset_remove(PSet *s, Point p){
    for(int i=0; i<s->size; i++){
        if(point_equal((s->points)[i], p))
            remove_point(s, i);
    }
    s->size--;
    if(s->size <= (s->max_size >> 1)){
        s->max_size >>= 1;
        s->points = realloc(s->points, s->max_size*sizeof(Point));
    }
}


Point *pset_to_array(PSet *s){
    return s->points;
}

Point *pset_copy_to_array(PSet *s){
    Point *points = malloc(s->size*sizeof(Point));
    memcpy(points, s->points, s->size*sizeof(Point));
    return points;
}

int pset_len(PSet *s){
    return s->size;
}

Point *pset_to_array_and_destroy(PSet *s){
    Point *points = s->points;
    free(s);
    return points;
}

PSet *pset_init_from_array(Point *points, int n){
    PSet *s = pset_init(n);
    memcpy(s->points, points, n*sizeof(Point)); 
    return s;
}


int pset_maxlen(PSet *s){
    return s->max_size;
}

int pset_contains(PSet *s, Point p){
    for(int i=0; i<s->size; i++){
        if (point_equal(p, (s->points)[i]))
            return 1;   
    }
    return 0;
}