#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "../../headers/point.hpp"
#include "../../headers/utils/random.hpp"
#include <vector>
#include <random>
#include <algorithm>


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


std::vector<Point> random_sample_generator(int size){
    std::vector<Point> points(size); // Create vector

    for(int i=0; i<size; i++)
        points[i] = random_point();

    return points;
}


std::vector<Point> random_k_sample(std::vector<Point> points, int k){
    std::vector<Point> sample(k);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::shuffle(points.begin(), points.end(), gen);
    std::vector<int> selected_elements(points.begin(), points.begin() + k);

    return sample;
}