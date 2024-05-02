#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../../headers/point.h"
#include "../../headers/utils/random_generator.h"


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
    srand(time(NULL));

    Point *points = (Point*) malloc(sample_size*sizeof(Point));

    for(int i=0; i<sample_size; i++)
        points[i] = random_point();

    return points;
}


Point* random_k_sample(Point *arr, int n, int k){
    srand(time(NULL));
    Point *sample = malloc(n*sizeof(Point));
    memcpy(sample, arr, n*sizeof(Point));

    for(Point* p=sample; p<sample+k; p++){
        int j = rand() % (n--);
        swap_points(p, p+j);
    }

    sample = realloc(sample, k*sizeof(Point));
    return sample;
}