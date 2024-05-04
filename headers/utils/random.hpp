#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "../point.hpp"
#include <vector>


/** Genera un Punto con coordenadas aleatorias
*   Las coordenadas estan en [0, 1]
*   
*   @return Point(x, y)
*/
Point random_point(void);

/** Genera un array de puntos
*   Los puntos tienen coordenadas con valores entre 0 y 1
*   
*   @param sample_size     El numero de puntos a generar
*/
std::vector<Point> random_sample_generator(int sample_size, int seed);

/** Returns an array of k random elements
*   from an array of size n
*/
std::vector<Point> random_k_sample(std::vector<Point> points, int k);

#endif