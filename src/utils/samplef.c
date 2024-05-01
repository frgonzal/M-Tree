#include <stdlib.h>
#include "../../headers/utils/samplef.h"
#include "../../headers/point.h"
#include "../../headers/utils/queue.h"
#include "../../headers/utils/vector.h"

struct sample_f {
    Vector *f;
    Vector *F;
    int size;
};

SampleF *samplef_init(int init_size){
    int max_size = 0x1;
    while(init_size >= max_size)
        max_size <<= 1;

    SampleF *f = malloc(sizeof(SampleF));
    *f = (SampleF){
        vec_init(0, Point),
        vec_init(0, Queue*),
        0
    };

    return f;
}

void samplef_destroy(SampleF *f){
    Queue **F = vec_to_array_and_destroy(f->F);
    for(int i=0; i<f->size; i++)
        q_destroy(F[i]);
    vec_destroy(f->f);
    free(f);
}


void samplef_push(SampleF *f, Point *p){
    vec_push(f->f, p);
    Queue *q = q_init();
    vec_push(f->F, &q);
    f->size++;
}

void samplef_assign_from_array(SampleF *f, Point *p, int n){
    return;
}

void samplef_assign_from_queue(SampleF *f, Queue *q){
    return;
}

Queue *samplef_remove(SampleF *f, Point *p){
    int i = vec_find(f->f, p, &cmp_points);
    //vec_remove(f->f, vec_find);
}

Queue *samplef_pop(SampleF *f, int i){

}

/** Returns the position of p */
//int samplef_find(SampleF *f, Point p);

/** Get f_i */
//int samplef_get(SampleF *f, int i);

/** Returns size of the sample */
int samplef_len(SampleF *f);

/** Returns the array F */
//Point *sampef_get_sample(SampleF *f);

/** Get the Queue of F_i */
Queue* samplef_get_queue_points(SampleF *f, int i);

/** Get the Array of F_i */
Point *samplef_get_array_points(SampleF *f, int i);