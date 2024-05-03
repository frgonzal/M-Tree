#ifndef SAMPLEF_H
#define SAMPLEF_H

#include "../point.h"
#include "queue.h"
#include "vector.h"

typedef struct sample_f SampleF;

/** Init the samplef */
SampleF *samplef_init(int init_size);

/** Init the sampleF from an array */
SampleF *samplef_init_from_array(Point const *points, int size);

/** Destroy the SampleF */
void samplef_destroy(SampleF *f);

/** Adds a point to the sample */
void samplef_push(SampleF *f, Point *p);

/** Assings points to its nearest f_k */
void samplef_assign_from_array(SampleF *f, Point const *points, int n);

/** Assings points to its nearest f_k */
void samplef_assign_from_vector(SampleF *f, Vector *v);

/** Assings points to its nearest
*   Starting at pos
*/
void samplef_assign_vector_strtpos(SampleF *f, Vector *v, int pos);

/** Removes the f[i], and return its points */
Vector *samplef_pop(SampleF *f, int i);

/** Returns the position of p */
int samplef_find(SampleF *f, Point *p);

/** Get f_i */
Point samplef_get(SampleF *f, int i);

/** Returns size of the sample */
int samplef_len(SampleF *f);

/** Returns the array F */
Vector *samplef_get_sample(SampleF *f);

/** Get the Array of F_i */
Vector *samplef_get_points(SampleF *f, int i);

/** Find the nearest f_i */
int samplef_find_nearest(SampleF *f, Point *p);

#endif