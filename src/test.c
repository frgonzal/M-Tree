#include <stdio.h>
#include "../headers/point.h"

int main(){
    printf("test\n");
    
    Point p1 = {0.02, -0.01};
    printf("p1 = (%.5f %.5f)\n", p1.x, p1.y);
    
    Point p2 = {0.01234, 0.1043};
    printf("p1 = (%.5f %.5f)\n", p2.x, p2.y);
    
    printf("dist(p1, p2) = %.5f\n", dist(p1, p2));

    return 0;
}