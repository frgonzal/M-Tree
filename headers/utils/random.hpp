#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "../point.hpp"
#include <vector>


/** Returns a random Point
*   x and y are between 0 and 1
*/
Point random_point(void);

/** Returns a random vector of size sample_size
*/
std::vector<Point> random_sample_generator(int sample_size, int seed);

/** Returns a vector of k random elements
*   from an array of size n
*/
std::vector<Point> random_k_sample(std::vector<Point> points, int k);

#endif