#include <complex.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../../headers/mtree.h"
#include "../../headers/point.h"
#include "../../headers/utils/queue.h"
#include "../../headers/utils/samplef.h"
#include "../../headers/utils/random_generator.h"


/** Agrega un array de puntos como hijos de un MTree */
static void add_childs(MTree *mtree, Point const *points, int n);


MTree bulk_loading(Point const *points, int n){
    // Si |P| ≤ B, se crea un árbol T , se insertan todos los puntos a T y se retorna T
    if(n <= B){
        MTree mtree = {points[0], 0, malloc(B*sizeof(MTree)), n, 1};
        add_childs(&mtree, points, n);
        return mtree;
    }


    SampleF *F;
    int k = (n/B < B) ? n/B : B; 
    do{
        // De manera aleatoria se eligen k = min(B, n/B) puntos de P, que los llamaremos samples pf1 , . . . , pfk . 
        // Se insertan en un conjunto F de samples.
        Point *sample_k = random_k_sample(points, n, k);
        //F = samplef_init();

        // Se le asigna a cada punto en P su sample más cercano. Con eso se puede construir k conjuntos
        // F1, . . . , Fk 
        

        // Etapa de redistribución: Si algún Fj es tal que |Fj| < b:
        //k = redistribute(F, k);

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


MTree* mtree_create_cp(Point const *points, int n){
    MTree *mtree = malloc(sizeof(MTree));
    *mtree = bulk_loading(points, n);
    return mtree;
}


/** Agrega hijos a un nodo MTree */
static void add_childs(MTree *mtree, Point const *points, int n){
    double max_dist2 = 0;

    for(int i=0; i<n; i++){
        double d2 = dist2(mtree->p, points[i]);
        max_dist2 = d2 > max_dist2 ? d2 : max_dist2;

        (mtree->a)[i] = (MTree){points[i], 0, NULL, 0, 0};
    }
    mtree->cr = sqrt(max_dist2); 
}