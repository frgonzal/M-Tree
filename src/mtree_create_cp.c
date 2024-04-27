#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../headers/mtree.h"
#include "../headers/point.h"


/** Agrega un array de puntos como hijos de un MTree */
static void add_childs(MTree *mtree, Point *points, int n);


/** Entrega un puntero a un array con k puntos randoms del array original.
*   Tambien modifica el array original, dejandolo solo con aquellos puntos
*   que no fueron seleccionados, ademas reduce el n en k.
*
*   La funcion guardara los puntos aleatorios en las primeras k posiciones
*   del array original. Luego retorna un puntero a la primera posicion y 
*   modifica el array original para que apunte k posiciones adelante.
*
*   @param points El array de puntos original
*   @param n      Puntero al largo del array
*   @param k      Largo del array de puntos aleatorios
*/
static Point *random_k(Point **points, int *n, int k);

static int max(int a, int b);

static int min(int a, int b);

static void swap_points(Point *p1, Point *p2);



MTree* cp_create(Point *points, int n){
    MTree *mtree = malloc(sizeof(MTree));

    // Si |P| ≤ B, se crea un árbol T , se insertan todos los puntos a T y se retorna T
    if(n <= B){
        *mtree = (MTree){points[0], 0, malloc(B*sizeof(MTree)), n-1};
        add_childs(mtree, points+1, n-1);
        return mtree;
    }

    // De manera aleatoria se eligen k = min(B, n/B) puntos de P, que los llamaremos samples pf1 , . . . , pfk . 
    // Se insertan en un conjunto F de samples.
    int k = min(B, n/B); 
    Point *p_f = random_k(&points, &n, k);

    // Se le asigna a cada punto en P su sample más cercano. Con eso se puede construir k conjuntos
    // F1, . . . , Fk 
    
    Point **F;




    return NULL;
}


static MTree* create_mtree(Point *points, int n) {
    MTree *mtree = malloc(sizeof(MTree));
    mtree->p = points[0];
    mtree->cr = 0;
    mtree->n = n - 1;
    if (mtree->n > 0) {
        mtree->a = malloc(mtree->n * sizeof(MTree));
        for (int i = 0; i < mtree->n; i++) 
            mtree->a[i] = (MTree){points[i+1], 0, NULL, 0};

    } else
        mtree->a = NULL;

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
static void add_childs(MTree *mtree, Point *points, int n) {
    double max_dist = 0;

    for(int i=0; i<n; i++){
        double dist_pq = dist(mtree->p, points[i]);
        max_dist = max(dist_pq, max_dist);

        (mtree->a)[i] = (MTree){points[i], 0, NULL, 0};
    }
}


static Point *random_k(Point **p_points, int *n, int k) {
    srand(time(NULL));
    Point *points = *p_points;
    
    for(int i=0; i<k; i++) {
        int j = rand() % *n;
        swap_points(points+i, points+j);
    }

    *n -= k;
    *p_points += k;
    return points;
}

static void swap_points(Point *p1, Point *p2) {
    Point temp = *p1;
    *p1 = *p2;
    *p2 = temp;
}

static int max(int a, int b) {
    return a > b ? a : b;
}


static int min(int a, int b) {
    return a < b ? a : b;
}