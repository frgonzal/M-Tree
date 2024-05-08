#ifndef POINT_HPP
#define POINT_HPP


/** @struct Point 
*/
struct Point{
    double x;   /*  x in [0, 1]  */
    double y;   /*  y in [0, 1]  */

    /** Compares two points
    *   @return True if both points have the same position
    */
    bool operator==(const Point &other) const;

    /** Compares two points 
    *   @return Comparison of the x values
    */
    bool operator<=(const Point &other) const;
    
    /** Compares two points 
    *   @return Comparison of the x values 
    */
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


#endif