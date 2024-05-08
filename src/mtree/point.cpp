#include <math.h>
#include "../../headers/point.hpp"


bool Point::operator==(const Point &other) const{
    return (x == other.x) && (y == other.y);
}

bool Point::operator<=(const Point &other) const{
    return (x != other.x) ? (x <= other.x) : (y <= other.y);
}

bool Point::operator<(const Point &other) const{
    return (x != other.x) ? (x < other.x) : (y < other.y);
}

double dist(Point p1, Point p2){
    double diff_x = p1.x - p2.x;
    double diff_y = p1.y - p2.y;

    return sqrt(diff_x*diff_x + diff_y*diff_y);
}

double dist2(Point p1, Point p2){
    double diff_x = p1.x - p2.x;
    double diff_y = p1.y - p2.y;

    return diff_x*diff_x + diff_y*diff_y;
}