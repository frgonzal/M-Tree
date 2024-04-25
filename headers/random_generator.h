#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

#include "point.h"

/** Genera un array de puntos
*   Los puntos tienen coordenadas con valores entre 0 y 1
*   
*   @param filename El nombre del archivo
*   @param size     El numero de puntos a generar
*/
Point* random_sample_generator(char *filename, int size);


#endif