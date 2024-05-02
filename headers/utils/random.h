#ifndef RANDOM_H
#define RANDOM_H

#include "../point.h"

/** Genera un array de puntos
*   Los puntos tienen coordenadas con valores entre 0 y 1
*   
*   @param sample_size     El numero de puntos a generar
*/
Point* random_sample_generator(int sample_size);

/** Returns an array of k random elements
*   from an array of size n
*/
Point* random_k_sample(Point const *arr, int n, int k);

#endif