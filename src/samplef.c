#include <stdlib.h>
#include "../headers/samplef.h"
#include "../headers/point.h"
#include "../headers/queue.h"

struct sample_f {
    Point *f;
    Queue **points;
    int size;
    int max_size;
};

SampleF *samplef_init(int init_size){
    int max_size = 0x1;
    while(init_size >= max_size)
        max_size <<= 1;

    SampleF *f = malloc(sizeof(SampleF));
    *f = (SampleF){
        malloc(max_size*sizeof(Point)),
        malloc(max_size*sizeof(Queue*)),
        0,
        max_size
    };
 
    return f;
}

void samplef_destroy(SampleF *f){
    for(int i=0; i<f->size; i++){
        //(f->points)[i];
    }
}

/** Adds a f_k */
void samplef_add_sample_point(SampleF *f, Point p, Queue* q);

/** Adds a point to the Queue */
void samplef_add_point(SampleF *f, Point p, Queue* q);

/** Removes the f[i] */
void samplef_remove(SampleF *f, Point p, int i);

/** Returns the f[i] == p */
int samplef_find(SampleF *f, Point p);

/** Returns the number of points on the Set */
int samplef_len(SampleF *f);

/** Get the F_i array of points */
Point *samplef_array_fi(SampleF *f, int i);