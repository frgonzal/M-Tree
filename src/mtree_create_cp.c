#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../headers/mtree.h"
#include "../headers/point.h"
#include "../headers/queue.h"


typedef struct {
    MTree mtree;
    int h;
} Tree;

typedef struct {
    Point f;
    Queue *points;
    int size;
} SampleF;


/** Entrega un puntero a un array con k puntos randoms del array original.
*   Tambien modifica el array original, dejandolo solo con aquellos puntos
*   que no fueron seleccionados, ademas reduce el n en k.
*
*   @param points array of points
*   @param n      number of points
*   @param k      length of sample
*/
static SampleF *random_sample_k(Point *points, int n, int k);

/** Agrega un array de puntos como hijos de un MTree */
static void add_childs(MTree *mtree, Point *points, int n);



MTree create_tree(Point *points, int n){


    SampleF *F;
    int k = (n/B < B) ? n/B : B; 
    do{
        // De manera aleatoria se eligen k = min(B, n/B) puntos de P, que los llamaremos samples pf1 , . . . , pfk . 
        // Se insertan en un conjunto F de samples.
        F = random_sample_k(points, n, k);

        // Se le asigna a cada punto en P su sample más cercano. Con eso se puede construir k conjuntos
        // F1, . . . , Fk 

        // Etapa de redistribución: Si algún Fj es tal que |Fj| < b:
        //k = redistribute(F);
    } while(k == 1);

    // Se realiza recursivamente el algoritmo CP en cada Fj, obteniendo el árbol T
    for (int i=0; i<k; i++){
        //MTree T_j = create_tree(F[i].points, F[i].size);
        //if (T_j.n < B/2){
            // split Tj into p sub-trees Tj ,..., Tj+p−1;
        //}
    }

    return (MTree){0,0,0,NULL,0};
}

MTree* mtree_create_cp(Point *points, int n){
    MTree *mtree = malloc(sizeof(MTree));

    // Si |P| ≤ B, se crea un árbol T , se insertan todos los puntos a T y se retorna T
    if(n <= B){
        *mtree = (MTree){points[0], 0, malloc(B*sizeof(MTree)), n-1};
        add_childs(mtree, points+1, n-1);
        return mtree;
    }

    *mtree = create_tree(points, n);
    return mtree;
}


static void assign_points_to_samples(Point *points, int n, Point *samples, int k, int *cluster_index) {
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

/** Agrega hijos a un nodo MTree */
static void add_childs(MTree *mtree, Point *points, int n){
    double max_dist = 0;

    for(int i=0; i<n; i++){
        double dist_pq = dist(mtree->p, points[i]);
        max_dist = dist_pq > max_dist ? dist_pq : max_dist;

        (mtree->a)[i] = (MTree){points[i], 0, NULL, 0};
    }
    mtree->cr = max_dist; 

}


/** Selecciona k puntos random de un array */
static SampleF *random_sample_k(Point *points, int n, int k) {
    srand(time(NULL));
    
    SampleF *F = malloc(k*sizeof(SampleF));

    for(int i=0; i<k; i++) {
        int j = rand() % (n-i);
        swap_points(points, points+j);
        points++;
    }

    for(int i=0; i<k; i++){

    }
        

    return F;
}