#ifndef SAMPLEF_H
#define SAMPLEF_H

#include "../point.h"
#include "queue.h"

typedef struct sample_f SampleF;

/** Init the Set */
SampleF *samplef_init(int init_size);

/** Destroy the Set */
void samplef_destroy(SampleF *f);

/** Adds a point to F */
void samplef_push(SampleF *f, Point *p);

/** Assings a point to its nearest f_k */
void samplef_assign_from_array(SampleF *f, Point *p, int n);

/** Assings a point to its nearest f_k */
void samplef_assign_from_queue(SampleF *f, Queue *q);

/** Removes the f[i], and return its queue of points */
Queue *samplef_remove(SampleF *f, Point *p);

Queue *samplef_pop(SampleF *f, int i);

/** Returns the position of p */
int samplef_find(SampleF *f, Point *p);

/** Get f_i */
Point* samplef_get(SampleF *f, int i);

/** Returns size of the sample */
int samplef_len(SampleF *f);

/** Returns the array F */
Point const *sampef_get_sample(SampleF *f);

/** Get the Queue of F_i */
Queue *samplef_get_queue_points(SampleF *f, int i);

/** Get the Array of F_i */
Point *samplef_get_array_points(SampleF *f, int i);


#endif