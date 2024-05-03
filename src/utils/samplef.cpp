#include <stdlib.h>
#include "../../headers/utils/samplef.hpp"
#include "../../headers/point.hpp"
#include "../../headers/utils/queue.hpp"
#include "../../headers/utils/vector.hpp"

struct sample_f {
    Vector *f;
    Vector *F;
};

SampleF *samplef_init(int init_size){
    SampleF *f = malloc(sizeof(SampleF));
    *f = (SampleF){
        vec_init(init_size, Point),
        vec_init(init_size, Vector*)
    };

    return f;
}

SampleF *samplef_init_from_array(Point const *points, int size){
    SampleF *f = malloc(sizeof(SampleF));
    *f = (SampleF){
        vec_init_from_array((void*)points, size, Point),
        vec_init(size, Vector*)
    };

    for(int i=0; i<size; i++){
        Vector *v = vec_init(size, Point);
        vec_push(f->F, &v);
    }

    return f;
}

void samplef_destroy(SampleF *f){
    for(int i=0; i<vec_len(f->F); i++){
        Vector **v = vec_get(f->F, i);
        vec_destroy(*v);
    }
    vec_destroy(f->F);
    vec_destroy(f->f);
    free(f);
}


void samplef_push(SampleF *f, Point *p){
    Vector *v = vec_init(0, Point);
    vec_push(f->f, p);
    vec_push(f->F, &v);
}


Vector *samplef_pop(SampleF *f, int pos){
    vec_remove(f->f, pos);
    return vec_pop_t(f->F, pos, Vector*);
}

int samplef_find(SampleF *f, Point *p){
    return vec_find(f->f, p, &cmp_points);
}

Point samplef_get(SampleF *f, int pos){
    return vec_get_t(f->f, pos, Point);
}

int samplef_len(SampleF *f){
    return vec_len(f->f);
}

Vector *samplef_get_sample(SampleF *f){
    return f->f;
}

Vector *samplef_get_points(SampleF *f, int pos){
    return vec_get_t(f->F, pos, Vector*);
}


static void samplef_assign_point(SampleF *f, Point *p, int j){
    double min_dist2 = 1e5;
    int k = -1;
    Point const *points = vec_to_array(f->f);
    for(int i=j; i<vec_len(f->f); i++){
        double d2 = dist2(*p, points[i]);
        if(d2 < min_dist2){
            k = i;
            min_dist2 = d2;
        }
    }
    Vector *v = samplef_get_points(f, k);
    vec_push(v, p);
}

void samplef_assign_from_array(SampleF *f, Point const *points, int n){
    for(int i=0; i<n; i++){
        Point const *p = points+i;

        double min_dist2 = 1e6;
        int k = -1;

        Point const *F = vec_to_array(f->f);
        int v_len = vec_len(f->f);

        for(int j=0; j<v_len; j++){
            double d2 = dist2(*p, F[j]);
            if(d2 < min_dist2){
                k = j;
                min_dist2 = d2;
            }
        }

        Vector *v = samplef_get_points(f, k);
        vec_push(v, (Point*)p);
    }
}

void samplef_assign_from_vector(SampleF *f, Vector *v){
    Point const* points = vec_to_array(v);
    samplef_assign_from_array(f, points, vec_len(v));
}

void samplef_assign_vector_strtpos(SampleF *f, Vector *v, int pos){
    for(int i=0; i<vec_len(v); i++){
        Point *p = (Point*)vec_get(v, i);
        samplef_assign_point(f, p, pos);
    }
}


int samplef_find_nearest(SampleF *f, Point *p){
    double min_dist2 = 1e5;
    int k = -1;
    Point const *points = vec_to_array(f->f);
    for(int i=0; i<vec_len(f->f); i++){
        double d2 = dist2(*p, points[i]);
        if(d2 < min_dist2){
            k = i;
            min_dist2 = d2;
        }
    }
    return k;
}
