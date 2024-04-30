#ifndef SAMPLEF_H
#define SAMPLEF_H

#include "point.h"
#include "queue.h"

typedef struct sample_f SampleF;

/** Init the Set */
SampleF *samplef_init(int init_size);

/** Destroy the Set */
void samplef_destroy(SampleF *f);

/** Adds a f_k */
void samplef_add_sample_point(SampleF *f, Point p, Queue* q);

/** Adds a point to the Queue */
void samplef_add_point(SampleF *f, Point p, Queue* q);

/** Removes the f[i] */
void samplef_remove(SampleF *f, Point p, int i);

/** Returns the f[i] == p */
int samplef_find(SampleF *f, Point p);

/** Returns the number of points on the Set */
int samplef_len(SampleF *f);

/** Get the F_i array of points */
Point *samplef_array_fi(SampleF *f, int i);


#endif