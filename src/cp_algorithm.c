#include <stdio.h>
#include <stdlib.h>
#include "../headers/mtree.h"
#include "../headers/point.h"




MTree* create_mtree(Point *points, int n) {
    MTree *mtree = malloc(sizeof(MTree));
    mtree->p = points[0];
    mtree->cr = 0;
    mtree->n = n - 1;
    if (mtree->n > 0) {
        mtree->a = malloc(mtree->n * sizeof(MTree));
        for (int i = 0; i < mtree->n; i++) {
            mtree->a[i].p = points[i + 1];
            mtree->a[i].cr = 0;
            mtree->a[i].a = NULL;
            mtree->a[i].n = 0;
        }
    } else {
        mtree->a = NULL;
    }
    return mtree;
}

void assign_points_to_samples(Point *points, int n, Point *samples, int k, int *cluster_index) {
    for (int i = 0; i < n; i++) {
        double min_dist = 99999;
        int clust_index = 0;
        for (int j = 0; j < k; j++) {
            double d = dist(points[i], samples[j]);
            if (d < min_dist) {
                min_dist = d;
                clust_index = j;
            }
        }
        cluster_index[i] = clust_index;
    }
}