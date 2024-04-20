#include <math.h>
#include "../Headers/Point.h"


double dist(Point p1, Point p2){
    double diff_x = p1.x - p2.x;
    double diff_y = p1.y - p2.y;

    return sqrt(diff_x*diff_x + diff_y*diff_y);
}