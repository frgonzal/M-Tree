#ifndef POINT_HPP
#define POINT_HPP


/** @struct Point 
*
*   Representa un punto en el plano
*/
struct Point{
    double x;
    double y;

    bool operator==(const Point &other) const;
    bool operator<=(const Point &other) const;
    bool operator<(const Point &other) const;
};

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

/** Compara en base a la coordenada X 
*   Retorna verdadero si p1 < p2
*/
int cmp_point_x(Point p1, Point p2);

/** Compara en base a la coordenada Y 
*   Retorna verdadero si p1 < p2
*/
int cmp_point_y(Point p1, Point p2);


#endif