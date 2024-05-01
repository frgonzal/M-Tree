#include <linux/limits.h>
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
    for(int i=0; i<f->size; i++)
        q_destroy((f->points)[i]);

    free(f->f);
    free(f->points);
}