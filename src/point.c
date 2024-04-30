#include <math.h>
#include "../headers/point.h"


double dist(Point p1, Point p2){
    double diff_x = p1.x - p2.x;
    double diff_y = p1.y - p2.y;

    return sqrt(diff_x*diff_x + diff_y*diff_y);
}

int point_equal(Point p1, Point p2){
    return (p1.x == p2.x) && (p1.y == p2.y);
}

void swap_points(Point *p1, Point *p2) {
    Point temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}