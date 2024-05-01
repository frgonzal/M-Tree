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


Queue *samplef_remove(SampleF *f, Point *p){
    int pos = vec_find(f->f, p, &cmp_points);
    vec_remove(f->f, pos);
    f->size--;
    return vec_pop_t(f->F, pos, Queue*);
}

Queue *samplef_pop(SampleF *f, int pos){
    f->size--;
    vec_remove(f->f, pos);
    return vec_pop_t(f->F, pos, Queue*);
}

int samplef_find(SampleF *f, Point *p){
    return vec_find(f->f, p, &cmp_points);
}

Point samplef_get(SampleF *f, int pos){
    return vec_get_t(f->f, pos, Point);
}

int samplef_len(SampleF *f){
    return f->size;
}

Point const *sampef_get_sample(SampleF *f){
    return (Point const *)vec_to_array(f->f);
}

Queue* samplef_get_queue_points(SampleF *f, int pos){
    return vec_get_t(f->F, pos, Queue*);
}

Point *samplef_get_array_points(SampleF *f, int pos){
    Queue *q = samplef_get_queue_points(f, pos);
    Point *points = malloc(vec_len(f->F)*sizeof(Point));

    for(int i=0; i<q_length(q); i++){
        Point *p = (Point*)q_get(q);
        points[i] = *p;
        q_put(q, &points[i]);
    }
    return points;
}


static void samplef_assign_point(SampleF *f, Point *p){
    //double dist2;
    //for(int i=0; i<f->size; i++){


    //}
    //Queue *q = samplef_get_queue_points(f, pos);
}

void samplef_assign_from_array(SampleF *f, Point *p, int n){
    return;
}

void samplef_assign_from_queue(SampleF *f, Queue *q){
    return;
}