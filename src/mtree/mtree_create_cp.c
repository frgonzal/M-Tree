#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../../headers/mtree.h"
#include "../../headers/point.h"
#include "../../headers/utils/queue.h"
#include "../../headers/utils/samplef.h"
#include "../../headers/utils/random.h"


/** Agrega un array de puntos como hijos de un MTree */
static void add_childs(MTree *mtree, Point const *points, int n);


MTree bulk_loading(Point const *points, int n){
    MTree mtree;

    // Si |P| ≤ B, se crea un árbol T , se insertan todos los puntos a T y se retorna T
    if(n <= B){
        mtree = (MTree){points[0], 0, malloc(B*sizeof(MTree)), n, 1};
        add_childs(&mtree, points, n);
        return mtree;
    }


    SampleF *F;
    int k = (n/B < B) ? n/B : B; 
    do{
        // De manera aleatoria se eligen k = min(B, n/B) puntos de P, que los llamaremos samples pf1 , . . . , pfk . 
        // Se insertan en un conjunto F de samples.
        Point *sample_k = random_k_sample(points, n, k);
        F = samplef_init_from_array(sample_k, n);
        free(sample_k);

        // Se le asigna a cada punto en P su sample más cercano. Con eso se puede construir k conjuntos
        // F1, . . . , Fk 
        samplef_assign_from_array(F, points, n);

        // Etapa de redistribución: Si algún Fj es tal que |Fj| < b:
        // a rezar que funcione lo de abajo
        for(int i=0; i<samplef_len(F); i++){
            Vector *v = samplef_get_points(F, i);
            if(vec_len(v) < (B >> 1)){
                samplef_pop(F, i);
                samplef_assign_from_vector(F, v);
                vec_destroy(v);
                i--;
            }
        }

        k = samplef_len(F);
    } while(k == 1);

    Vector *T = vec_init(k, MTree);
    Vector *T_prima = vec_init(k, MTree);
    
    // Se realiza recursivamente el algoritmo CP en cada Fj, obteniendo el árbol T
    for (int i=0; i<k; i++){
        Vector *v = samplef_get_points(F, i);
        int size = vec_len(v);
        Point *points_t = vec_to_array_and_destroy(v);
        MTree t = bulk_loading(points_t, n);

        // Si la raíz del árbol es de un tamaño menor a b
        if(t.n < (B >> 1)){
            //, se quita esa raíz, se elimina pfj de F y se trabaja con sus subárboles como nuevos 
            // Tj , . . . , Tj+p−1, se añaden los puntos pertinentes a F.
            // hijos = ...
            // vec_push(hijos)
        }else
            vec_push(T, &t);
    }
    // Etapa de balanceamiento: Se define h como la altura mínima de los árboles Tj. 
    // buscar altura minima entre los hijos
    // Se define T' inicialmente como un conjunto vacío.

    //Por cada Tj , si su altura es igual a h, se añade a T'
    //Si no se cumple:
        //Se borra el punto pertinente en F.
        //Se hace una búsqueda exhaustiva en Tj de todos los subárboles T'_1, . . . ,T'_p 
        // de altura igual a h. Se insertan estos árboles a T'
        // Se insertan los puntos raíz de T'_1, . . .,T'_p, f1, . . . ,fp en F

    // Se define T_sup == mtree como el resultado de la llamada al algoritmo CP aplicado a F
    Vector *T_sup = samplef_get_sample(F);
    mtree = bulk_loading((Point const *)vec_to_array(T_sup), k);

    //Se une cada Tj ∈ T' a su hoja en T_sup correspondiente al punto pfj ∈ F, 
    // obteniendo un nuevo árbol T .

    //Se setean los radios cobertores resultantes para cada entrada en este árbol.
    // ademas la altura = h_min + 1

    // liberar memoria utilizada
    vec_destroy(T);
    vec_destroy(T_prima);
    samplef_destroy(F);
    return mtree;
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