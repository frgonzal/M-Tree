#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../headers/point.h"
#include "../headers/random_generator.h"


/** Genera un array de puntos con coordenadas aleatorias
*   Las coordenadas estan en [0, 1]
*   
*   @param size el numero de puntos a generar
*
*   @return un puntero al array de Points
*/
static Point* random_points_generator(int size){
    Point *points = (Point*) malloc(size*sizeof(Point));

    for (int i=0; i<size; i++){
        double x  = (double) rand() / (double) (RAND_MAX-1);
        double y  = (double) rand() / (double) (RAND_MAX-1);
        points[i] = (Point){x, y};
    }

    return points;
}


void random_sample_generator(char *filename, int size){
    srand(time(NULL));
    FILE *filePtr;

    filePtr = fopen(filename, "wb");
    if (filePtr == NULL) {
        fprintf(stderr, "Error opening file.\n");
        exit(0);
    }

    Point *points = random_points_generator(size);

    size_t itemsWritten = fwrite(points, sizeof(Point), size, filePtr);

    if (itemsWritten != size) {
        fprintf(stderr, "Failed to write the correct amount of data.\n");
        fclose(filePtr);
        exit(0);
    }

    fclose(filePtr);

    return;
}
