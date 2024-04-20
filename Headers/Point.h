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

/** Calcula distancia entre 2 puntos 
*   @param p1 punto 1
*   @param p2 punto 2
*   
*   @return distancia entre los puntos
*/
double dist(Point p1, Point p2);

#endif