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

int point_equal(Point p1, Point p2){
    return (p1.x == p2.x) && (p1.y == p2.y);
}

void swap_points(Point *p1, Point *p2) {
    Point temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

int cmp_points(void *pp1, void *pp2){
    Point p1 = *(Point*) pp1;
    Point p2 = *(Point*) pp2;
    return (p1.x == p2.x && p1.y == p2.y) ? 0 : -1;
}

int cmp_point_x(Point p1, Point p2){
    return (p1.x != p2.x) ? (p1.x <= p2.x) : (p1.y <= p2.y);
}

int cmp_point_y(Point p1, Point p2){
	return (p1.y != p2.y) ? (p1.y <= p2.y) : (p1.x <= p2.x);
}


