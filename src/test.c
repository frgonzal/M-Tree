#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../headers/point.h"
#include "../headers/utils/random.h"
#include "../headers/utils/vector.h"

#define MAX 33554432


// Function to calculate elapsed time in milliseconds
long long elapsed_time_millis(struct timespec *start, struct timespec *end) {
    long long start_ms = start->tv_sec * 1000LL + start->tv_nsec / 1000000LL;
    long long end_ms = end->tv_sec * 1000LL + end->tv_nsec / 1000000LL;
    return end_ms - start_ms;
}


static void print_array(Point const *points, int n){
    printf("[");
    for(int i=0; i<n; i++){
        printf("(%.2f, %.2f)",points[i].x, points[i].y);
        if(i < n-1)
            printf(", ");
    }
    printf("]\n");
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
    print_array(points, 3);
    printf("time: %lld\n", duration);

    printf("random k sample test\n");

    clock_gettime(CLOCK_MONOTONIC, &start);
    Point *points_k = random_k_sample(points, MAX, 5);
    clock_gettime(CLOCK_MONOTONIC, &end);
    duration = elapsed_time_millis(&start, &end);
    printf("time: %lld\n", duration);
    print_array(points_k, 3);

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
    print_array(vec_to_array_t(vec1, Point), 10);


    printf("== find and remove ==\n");

    int i = vec_find(vec1, points, &cmp_points);
    printf("find (%.2f, %.2f) %d\n", points->x, points->y, i);
    vec_remove(vec1, i);
    print_array(vec_to_array_t(vec1, Point), 5);
    printf("size: %d\n", vec_len(vec1));


    printf("== get element ==\n");
    Point p1 = vec_get_t(vec1, 0, Point);
    Point p2 = *(Point*)vec_get(vec1, 1);
    print_array(&p1, 1);
    print_array(&p2, 1);

    printf("== copy to array ==\n");
    free(points);
    points = (Point*)vec_copy_to_array(vec1);
    
    printf("== to array and destroy ==\n");
    free(points);
    points = (Point*)vec_to_array_and_destroy(vec1);
     
    printf("== pop ==\n");

    vec1 = vec_init_from_array(points, 5, Point);
    printf("size: %d\n", vec_len(vec1));
    print_array(vec_to_array(vec1), vec_len(vec1));    

    Point x = vec_pop_t(vec1, 0, Point);
    vec_pop_t(vec1, 2, Point);

    printf("x = ");
    print_array(&x, 1);    
    print_array(vec_to_array(vec1), vec_len(vec1));    

    free(points);
    vec_destroy(vec1);

}


int main(){
    printf("=====  TEST  =====\n");

    //random_sample_generator_test(2);

    vector_test();

    random_test(1e5);



    return 0;
}