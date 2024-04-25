#include <stdio.h>
#include <stdlib.h>
#include "../headers/point.h"
#include "../headers/random_generator.h"


int main(){
    printf("random sample generator test\n");

    int n = 10;

    Point* points = random_sample_generator(n);

    for(int i=0; i<n; i++)
        printf("Point: %.5f %.5f \n", points[i].x, points[i].y);

    free(points);

    return 0;
}