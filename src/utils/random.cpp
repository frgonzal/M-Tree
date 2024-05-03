#include <stdlib.h>
#include <sys/time.h>
#include "../../headers/point.hpp"
#include "../../headers/utils/random.hpp"
#include <vector>
#include <random>
#include <algorithm>

/** Random generator */
static std::mt19937 gen;

/** To initialize the random seed */
static void init_random(){
    static int random_init = 0;
    if(!random_init){
        std::random_device rd;
        std::mt19937 gen(rd());
        srand(rd());
    }
}


Point random_point(void){
    init_random();

    double x  = (double) rand() / (double) (RAND_MAX-1);
    double y  = (double) rand() / (double) (RAND_MAX-1);
    return (Point){x, y};
}


std::vector<Point> random_sample_generator(int size){
    init_random();
    std::vector<Point> points(size); // Create vector

    for(int i=0; i<size; i++){
        double x  = (double) rand() / (double) (RAND_MAX-1);
        double y  = (double) rand() / (double) (RAND_MAX-1);
        points[i] = (Point){x, y};
    }

    return points;
}


std::vector<Point> random_k_sample(std::vector<Point> points, int k){
    init_random();

    std::shuffle(points.begin(), points.end(), gen);
    std::vector<Point> selected_elements(points.begin(), points.begin() + k);

    return selected_elements;
}