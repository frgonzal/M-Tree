#include <stdio.h>
#include <stdlib.h>
#include "../headers/point.h"
#include "../headers/random_generator.h"
#include "../headers/point_set.h"

static void print_array(Point *points, int n){
    printf("[");
    for(int i=0; i<n; i++){
        printf(" (%.2f, %.2f), ",points[i].x, points[i].y);
        if(i < n-1)
            printf(", ");
    }
    printf("]\n");
}


void random_sample_generator_test(int n){
    printf("random sample generator test\n");

    Point* points = random_sample_generator(n);

    for(int i=0; i<n; i++)
        printf("Point: %.5f %.5f \n", points[i].x, points[i].y);

    free(points);
}

void pset_test(){
    printf("\n=== Point Set Test ===\n");

    int n = 25;
    PSet *s;
    Point *points = random_sample_generator(n);

    s = pset_init(n);

    Point *points2 = pset_to_array(s);

    printf("Agregando %d puntos\n", n);
    for(int i=0; i<n; i++)
        pset_push(s, points[i]);
    points2 = pset_to_array(s);
    print_array(points2, pset_len(s));

    int k = 10;
    printf("Removiendo punto (%.2f,%.2f)\n", points2[k].x, points2[k].y);
    pset_remove(s, points2[k]);
    points2 = pset_to_array(s);

    while(pset_len(s) >= 20){
        pset_remove(s, points2[0]);        
    }
    points2 = pset_to_array(s);
    print_array(points, n);


    n = 1e3;
    printf("test de memoria\n");
    points = random_sample_generator(n);

    pset_init_from_array(points, n);

    free(points);
    int size = pset_len(s);
    points = pset_to_array_and_destroy(s);

    s = pset_init_from_array(points, size);

    free(points);
    points = pset_copy_to_array(s);

    free(points);
    pset_destroy(s);
}



int main(){

    random_sample_generator_test(2);
    pset_test();

    return 0;
}