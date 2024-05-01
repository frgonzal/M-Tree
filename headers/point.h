#ifndef POINT_H
#define POINT_H


/** @struct Point 
*
*   Representa un punto en el plano
*/
typedef struct {
    double x;
    double y;
} Point;

/** Distance between two points
*
*   @param p1
*   @param p2
*   
*   @return Distance
*/
double dist(Point p1, Point p2);

/** Square distance between two points 
*
*   @param p1
*   @param p2
*   
*   @return Distance
*/
double dist2(Point p1, Point p2);


/** Returns true if the distance between two points is 0 */
int point_equal(Point p1, Point p2);

/** Swaps to points on an array */
void swap_points(Point *p1, Point *p2);

/** Compares to points
*   useful for vector
*/
int cmp_points(void *pp1, void *pp2);

#endif