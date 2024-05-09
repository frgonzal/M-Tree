/**
*	A divide and conquer program in C++ to find the smallest distance from a
*	given set of points.
*
* 	The algorithm was modified to work properly for this proyect
*
*	@refer: https://www.geeksforgeeks.org/closest-pair-of-points-onlogn-implementation/
*/


#include <stdlib.h>
#include <float.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include "../../headers/point.hpp"
#include "../../headers/utils/closest_pair.hpp"


/** To save the original position of the Point */
typedef struct {
	double x, y;
	int pos;
} PointPos;


// Needed to sort array of points according to X coordinate
static int compareX(const void* a, const void* b){
	PointPos *p1 = (PointPos *)a,  *p2 = (PointPos *)b;
    double diff = (p1->x != p2->x) ? (p1->x - p2->x) : (p1->y - p2->y);
    if(diff == 0) return 0;
    return (diff > 0)? 1 : -1;
}
// Needed to sort array of points according to Y coordinate
static int compareY(const void* a, const void* b){
	PointPos *p1 = (PointPos *)a,  *p2 = (PointPos *)b;
	double diff = (p1->y != p2->y) ? (p1->y - p2->y) : (p1->x - p2->x);
    if(diff == 0) return 0;
    return (diff > 0)? 1 : -1;
}


// A Brute Force method to return the smallest distance between two points
// in P[] of size n
static std::tuple<int, int, double> bruteForce(PointPos *P, int n) {
    double min = DBL_MAX;
    int a = 0, b = 0;

    for (int i = 0; i < n; ++i){
        for (int j = i+1; j < n; ++j){
			Point p1 = {P[i].x, P[i].y};
			Point p2 = {P[j].x, P[j].y};
			double d = dist(p1, p2);
            if (d < min){
                min = d;
                a = P[i].pos;
                b = P[j].pos;
            }
		}
	}
    return std::make_tuple(a, b, min);
}


// A utility function to find the distance between the closest points of
// strip of a given size. All points in strip[] are sorted according to
// y coordinate. They all have an upper bound on minimum distance as d.
// Note that this method seems to be a O(n^2) method, but it's a O(n)
// method as the inner loop runs at most 6 times
static std::tuple<int, int, double> stripClosest(PointPos *strip, int size, double d, int pos_i, int pos_j) {
	double min = d; // Initialize the minimum distance as d
    int a = pos_i, b = pos_j;

	// Pick all points one by one and try the next points till the difference
	// between y coordinates is smaller than d.
	// This is a proven fact that this loop runs at most 6 times
	for (int i = 0; i < size; ++i){
		for (int j = i+1; j < size && (strip[j].y - strip[i].y) < min; ++j){
			Point p1 = {strip[i].x, strip[i].y};
			Point p2 = {strip[j].x, strip[j].y};
			double distance = dist(p1, p2);
			if (distance < min){
				min = distance;
                a = strip[i].pos;
                b = strip[j].pos;
            }
		}
	}

	return std::make_tuple(a, b, min);
}

// A recursive function to find the smallest distance. The array Px contains
// all points sorted according to x coordinates and Py contains all points
// sorted according to y coordinates
static std::tuple<int, int, double> closestUtil(PointPos *Px, PointPos *Py, int n)
{
	// If there are 2 or 3 points, then use brute force
	if (n <= 3)
		return bruteForce(Px, n);

	// Find the middle point
	int mid = n/2;
	PointPos midPoint = Px[mid];


	// Divide points in y sorted array around the vertical line.
	// Assumption: All x coordinates are distinct.
	PointPos *Pyl = (PointPos*)malloc(mid*sizeof(PointPos)); // y sorted points on left of vertical line
	PointPos *Pyr = (PointPos*)malloc((n-mid)*sizeof(PointPos)); // y sorted points on right of vertical line

	int li = 0, ri = 0; // indexes of left and right subarrays
	for (int i = 0; i < n; i++) {
        //std::cout << li << " " << ri << std::endl;
        if ((Py[i].x < midPoint.x || (Py[i].x == midPoint.x && Py[i].y < midPoint.y)) && li < mid)
            Pyl[li++] = Py[i];
        else
            Pyr[ri++] = Py[i];
	}

	// Consider the vertical line passing through the middle point
	// calculate the smallest distance dl on left of middle point and
	// dr on right side
	std::tuple<int, int, double> dl = closestUtil(Px, Pyl, mid);
	std::tuple<int, int, double> dr = closestUtil(Px + mid, Pyr, n-mid);
    free(Pyl); 
    free(Pyr); 

	// Find the smaller of two distances
	double d;
    int pos_i;
    int pos_j;
    if( std::get<2>(dl) < std::get<2>(dr)){
        std::tie(pos_i, pos_j, d) = dl;
    }else{
        std::tie(pos_i, pos_j, d) = dr;
    }

	// Build an array strip[] that contains points close (closer than d)
	// to the line passing through the middle point
	PointPos *strip = (PointPos*)malloc(n*sizeof(PointPos));;
	int j = 0;
	for (int i = 0; i < n; i++)
		if (abs(Py[i].x - midPoint.x) < d)
			strip[j] = Py[i], j++;

	// Find the closest points in strip. Return the minimum of d and closest
	// distance is strip[]
    std::tuple<int, int, double> tup = stripClosest(strip, j, d, pos_i, pos_j);
    free(strip);

    return tup;
}

// The main function that finds the smallest distance
// This method mainly uses closestUtil()
std::tuple<int, int> closest(const std::vector<Point> &P) {

	/** The algorithm uses Arrays */
	PointPos *Px = (PointPos*)malloc(sizeof(PointPos)*P.size());
	PointPos *Py = (PointPos*)malloc(sizeof(PointPos)*P.size());

	for (int i = 0; i < P.size(); i++) {
		Px[i] = {P[i].x, P[i].y, i};
		Py[i] = {P[i].x, P[i].y, i};
	}

	qsort(Px, P.size(), sizeof(PointPos), compareX);
	qsort(Py, P.size(), sizeof(PointPos), compareY);

	// Use recursive function closestUtil() to find the smallest distance
    std::tuple<int, int, double> tup = closestUtil(Px, Py, P.size());

    free(Px);
    free(Py);
	return std::make_tuple(std::get<0>(tup), std::get<1>(tup));
}
