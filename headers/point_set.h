#ifndef POINT_SET_H
#define POINT_SET_H

#include "point.h"

/** @struct PSet
*   
*   A set of points with dynamic size.
*/
typedef struct point_set PSet;


/** Init the Set 
*   
*   @param init_size The initial size available
*   
*   @return A Set of points
*/
PSet *pset_init(int init_size);

/** Init the Set from an array.
*   Copy the array into the Set.
*   
*   @param points   array of points
*   @param n        number of points 
* 
*   @return A Set of points
*/
PSet *pset_init_from_array(Point *points, int n);

/** Destroy the Set */
void pset_destroy(PSet *s);

/** Push a point into the Set */
void pset_push(PSet *s, Point p);

/** Removes a point from the Set 
*   It searches for the point on the Set
*/
void pset_remove(PSet *s, Point p);

/** Search for a point on a Set 
*
*   @return True if the point is on the Set
*/
int  pset_contains(PSet *s, Point p);

/** Returns the number of points on the Set */
int pset_len(PSet *s);

/** Get the pointer to the array of points 
*   warning: do not modify the array
*    
*   @return a pointer to an array of points
*/
Point *pset_to_array(PSet *s);

/** Get a copy of the points on the Set 
*
*   Slower than pset_to_array because of the copy
*   but safer
*
*   @return a pointer to an array of points
*/
Point *pset_copy_to_array(PSet *s);

/** Get the pointer to the array of points
*   and then destroy the Set.
*
*   Could be useful if only want the array
*/
Point *pset_to_array_and_destroy(PSet *s);

/** Returns the max size 
*   useful for testing
*/
int pset_maxlen(PSet *s);


#endif