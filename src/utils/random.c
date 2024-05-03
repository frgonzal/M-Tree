#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "../../headers/point.h"
#include "../../headers/utils/random.h"


/** Genera un Punto con coordenadas aleatorias
*   Las coordenadas estan en [0, 1]
*   
*   @return Point(x, y)
*/
static Point random_point(void){
    double x  = (double) rand() / (double) (RAND_MAX-1);
    double y  = (double) rand() / (double) (RAND_MAX-1);

    return (Point){x, y};
}


Point* random_sample_generator(int sample_size){
    Point *points = (Point*) malloc(sample_size*sizeof(Point));

    for(int i=0; i<sample_size; i++)
        points[i] = random_point();

    return points;
}


Point* random_k_sample(Point const *arr, int n, int k){
    Point *sample = malloc(n*sizeof(Point));
    sample = memcpy(sample, arr, n*sizeof(Point));

    for(int i=0; i <k ; i++){
        int j = i + rand() % (n-i);
        swap_points(sample+i, sample+j);
    }

    sample = realloc(sample, k*sizeof(Point));
    if(sample == NULL){
        printf("Could not create random k sample");
        exit(1);
    }
    return sample;
}