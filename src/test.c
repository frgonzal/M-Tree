#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../headers/point.h"
#include "../headers/mtree.h"
#include "../headers/utils/random.h"
#include "../headers/utils/queue.h"
#include "../headers/utils/vector.h"
#include "../headers/utils/samplef.h"

//#define MAX 33554432
#define MAX 33554


// Function to calculate elapsed time in milliseconds
long long elapsed_time_millis(struct timespec *start, struct timespec *end) {
    long long start_ms = start->tv_sec * 1000LL + start->tv_nsec / 1000000LL;
    long long end_ms = end->tv_sec * 1000LL + end->tv_nsec / 1000000LL;
    return end_ms - start_ms;
}


static void printf_array(Point const *points, int n){
    printf("[");
    for(int i=0; i<n; i++){
        printf("(%.2f, %.2f)",points[i].x, points[i].y);
        if(i < n-1)
            printf(", ");
    }
    printf("]\n");
}

static void printf_sample(SampleF *f){
    for(int i=0; i<samplef_len(f); i++){
        Point p = samplef_get(f, i);
        Vector *v = samplef_get_points(f, i);
        printf("f[%d] =  (%.2f, %.2f)\n", i, p.x, p.y);
        printf("F[%d] = ", i);
        printf_array(vec_to_array(v), vec_len(v));
    }   

}

static void printf_mtree_node(MTree *mtree){
    printf("p=(%.2f,%.2f);h=%d;cr=%1.2f;n=%d]", mtree->p.x, mtree->p.y, mtree->h, mtree->cr, mtree->n);
}

static void printf_mtree(MTree *mtree){
    if(mtree == NULL)
        return;
    Queue *q = q_init();
    q_put(q, mtree);
    int min_h = mtree->h;

    while(!q_empty(q)){
        MTree* mtree = q_get(q);
        //printf("\n");
        printf_mtree_node(mtree);
        printf("\n");
        //printf(" (%.2f, %.2f) ", mtree->p.x, mtree->p.y);
        for(int i=0; i<mtree->n; i++){
            q_put(q, mtree->a+i);
        }
    }
    q_destroy(q);
    printf("\n");
}


void random_test(int n){
    struct timespec start, end;
    long long duration;

    printf("\n== random test ==\n");

    printf("random sample generator test\n");
    clock_gettime(CLOCK_MONOTONIC, &start);
    Point *points = random_sample_generator(MAX);
    clock_gettime(CLOCK_MONOTONIC, &end);
    duration = elapsed_time_millis(&start, &end);
    printf_array(points, 3);
    printf("time: %lld\n", duration);

    printf("random k sample test\n");

    clock_gettime(CLOCK_MONOTONIC, &start);
    Point *points_k = random_k_sample(points, MAX, 5);
    clock_gettime(CLOCK_MONOTONIC, &end);
    duration = elapsed_time_millis(&start, &end);
    printf("time: %lld\n", duration);
    printf_array(points_k, 3);

    printf("\n\tTest random k sample 2\n");
    {
        Point *points = random_sample_generator(10);
        for(int i=0; i<100; i++){
            Point *sample = random_k_sample(points, 10, 3);
            printf_array(sample, 3);
            free(sample);
        }
        free(points);
    }

    free(points_k);
    free(points);
}


void vector_test(){
    time_t t1, t2;

    printf("\n=== vector test ===\n");
    int n = MAX; // == 2^25
    Vector *vec1 = vec_init(0, Point);
    Point *points = random_sample_generator(n);

    printf("== pushing elements to the vector ==\n");
    t1 = time(NULL);
    for(int i=0; i<n; i++)
        vec_push(vec1, points+i);
    t2 = time(NULL);

    printf("time pushing elems: %ld\n", t2-t1);
    
    vec_destroy(vec1);

    printf("== init from array ==\n");
    vec1 = vec_init_from_array(points, n, Point);
    printf_array(vec_to_array_t(vec1, Point), 10);


    printf("== find and remove ==\n");

    int i = vec_find(vec1, points, &cmp_points);
    printf("find (%.2f, %.2f) %d\n", points->x, points->y, i);
    vec_remove(vec1, i);
    printf_array(vec_to_array_t(vec1, Point), 5);
    printf("size: %d\n", vec_len(vec1));


    printf("== get element ==\n");
    Point p1 = vec_get_t(vec1, 0, Point);
    Point p2 = *(Point*)vec_get(vec1, 1);
    printf_array(&p1, 1);
    printf_array(&p2, 1);

    printf("== copy to array ==\n");
    free(points);
    points = (Point*)vec_copy_to_array(vec1);
    
    printf("== to array and destroy ==\n");
    free(points);
    points = (Point*)vec_to_array_and_destroy(vec1);
     
    printf("== pop ==\n");

    vec1 = vec_init_from_array(points, 5, Point);
    printf("size: %d\n", vec_len(vec1));
    printf_array(vec_to_array(vec1), vec_len(vec1));    

    Point x = vec_pop_t(vec1, 0, Point);
    vec_pop_t(vec1, 2, Point);

    printf("x = ");
    printf_array(&x, 1);    
    printf_array(vec_to_array(vec1), vec_len(vec1));    

    free(points);
    vec_destroy(vec1);
}



void samplef_test(int k, int n){
    printf("\n\tSampleF test\n");
    printf("\n- Test creacion y asignacion\n");
    Point* points = random_sample_generator(n);
    Point* sample = random_k_sample(points, n, k);
    SampleF *f;
    Vector *v;
    {
        f = samplef_init(k);
        for(int i=0; i<k; i++)
            samplef_push(f, sample+i);
        samplef_assign_from_array(f, points, n);
        v = samplef_get_sample(f);
        printf("sample:\n");
        printf_array(vec_to_array(v), vec_len(v));

        printf_sample(f);
        samplef_destroy(f);
    }
    printf("\n- Creacion a partir de un array\n");
    {
        f = samplef_init_from_array(sample, k);
        v = vec_init_from_array(points, n, Point);
        samplef_assign_from_vector(f, v);
        printf_sample(f);

        samplef_destroy(f);
        vec_destroy(v);
    }
    printf("\n- asignacion a partir de vector con posicion inicial\n");
    {
        f = samplef_init_from_array(sample, k);
        v = vec_init_from_array(points, n, Point);
        samplef_assign_vector_strtpos(f, v, k-1);
        printf_sample(f);
        samplef_destroy(f);
        vec_destroy(v);
    }
    printf("\nfind get and pop\n");
    {
        f = samplef_init_from_array(sample, k);
        samplef_assign_from_array(f, points, n);
        for(int i=0; i<samplef_len(f); i++){
            Point p = samplef_get(f, i);
            if(i != samplef_find(f, &p)){
                printf("expected %d, obtained %d\n", i, samplef_find(f, &p));
                exit(1);
            }
        }
        while(samplef_len(f)>0){
            printf("F => ");
            v = samplef_pop(f, 0);
            Point const *x = vec_to_array(v);
            printf_array(x, vec_len(v));
            vec_destroy(v);
        }
        samplef_destroy(f);
    }
    free(points);
    free(sample);

    printf("\n Test de sobrecarga \n");
    {
        points = random_sample_generator(MAX);
        sample = random_k_sample(points, MAX, B);
        f = samplef_init_from_array(sample, B);
        samplef_assign_from_array(f, points, MAX);
        printf("samplef len: %d\n", samplef_len(f));

        samplef_destroy(f);
        free(points);
        free(sample);
    }
}

void mtree_cp_test(int n){
    printf("\n\tMetodo CP \n");
    MTree* mtree;
    Point* points = random_sample_generator(n);

    printf("Creando MTree...\n");
    mtree = mtree_create_cp(points, n);

    printf(" MTree:\n");
    printf_mtree(mtree);

    printf("\n- Search for all elements\n");
    {
        Point q = {0.5, 0.5};
        double r = 0.5;
        MTreeSearch ms = mtree_search(mtree, q, r);
        printf("I/Os: %d\n", ms.ios);
        printf_array(ms.points, ms.result_size);

        free(ms.points);
    }

    mtree_destroy(mtree);
}


int main(){
    srand(time(NULL));
    printf("\t=====  TEST  =====\n");

    //random_sample_generator_test(2);

    //vector_test();

    //random_test(10);

    //samplef_test(4, 10);

    mtree_cp_test(8);


}