#include <cstdlib>
#include <cmath>
#include <vector>
#include <tuple>
#include "../../headers/mtree.hpp"
#include "../../headers/point.hpp"
#include "../../headers/utils/random.hpp"


/** Agrega un array de puntos como hijos de un MTree */
static void add_childs(MTree *mtree, const std::vector<Point> &points);

static void assign_to_nearest(std::vector<Point, std::vector<Point>> &F, std::vector<Point> &points);
//static void redistribution(SampleF *f);

//static void update_radius(MTree *mtree);

//static void find_mtree_h(MTree *mtree, Vector *v, int h);

MTree bulk_loading(const std::vector<Point> &points){
    MTree mtree;

    // Si |P| ≤ B, se crea un árbol T , se insertan todos los puntos a T y se retorna T
    if(points.size() <= B){
        mtree = (MTree){points[0], 0, (MTree*)malloc(B*sizeof(MTree)), (int)points.size(), 1};
        add_childs(&mtree, points);
        return mtree;
    }


    std::vector<std::tuple<Point, std::vector<Point>>> F;

    int k;
    for(;;){
        std::vector<Point> sample_k;
        k = B;//(n/B < B) ? n/B : B; 

        // De manera aleatoria se eligen k = min(B, n/B) puntos de P, que los llamaremos samples pf1 , . . . , pfk . 
        // Se insertan en un conjunto F de samples.
        sample_k = random_k_sample(points, k);
        for(int i=0; i<k; i++){
            F.push_back(std::make_tuple(sample_k[i], std::vector<Point>(0)));
        }

        // Se le asigna a cada punto en P su sample más cercano. Con eso se puede construir k conjuntos
        // F1, . . . , Fk 
        //assign_to_nearest(F, points);

        // Etapa de redistribución: Si algún Fj es tal que |Fj| < b:
        //redistribution(F);

        k = F.size();

        if(k > 1)
            break;
    }

    std::vector<MTree> T;
    // Se realiza recursivamente el algoritmo CP en cada Fj, obteniendo el árbol T
    for (int i=0; i<k; i++){
        MTree T_k = bulk_loading(std::get<1>(F[i]));

        if(T_k.n >= (B >> 1)){
            T.push_back(T_k);
        }else{ 
            // Si la raíz del árbol es de un tamaño menor a b
            // Se quita esa raíz se elimina pfj de F
            F_k = samplef_pop(F, i);
            int start_pos = samplef_len(F);
            // Se trabaja con sus subárboles como nuevos 
            // Tj , . . . , Tj+p−1, se añaden los puntos pertinentes a F.
            for(int j=0; j<T_k.n; j++){
                samplef_push(F, &(T_k.a[j].p));
                // asignar solo a los recien llegados
                samplef_assign_vector_strtpos(F, F_k, start_pos);
                // agregar T_k a T
                vec_push(T, &(T_k.a[j]));

                vec_destroy(F_k);
            }
            free(T_k.a);
            i--;
        }
    }


    MTree *childs = (MTree*)vec_to_array(T);
    // Etapa de balanceamiento: Se define h como la altura mínima de los árboles Tj. 
    int min_h = 1e8;

    // buscar altura minima entre los hijos
    for(int i=0; i<vec_len(T); i++)
        min_h = childs[i].h < min_h ? childs[i].h : min_h;

    // Se define T' inicialmente como un conjunto vacío.
    Vector *T_prima = vec_init(vec_len(T), MTree);

    //Por cada Tj , si su altura es igual a h, se añade a T'
    for(int i=0; i<vec_len(T); i++){
        if(childs[i].h == min_h){
            vec_push(T_prima, vec_get(T, i));
        }else{ //Si no se cumple:
            //Se borra el punto pertinente en F.
            int pos = samplef_find_nearest(F, &childs[i].p);
            Vector *v = samplef_pop(F, pos);
            //Se hace una búsqueda exhaustiva en Tj de todos los subárboles T'_1, . . . ,T'_p 
            // de altura igual a h. 
            Vector *mtree_h = vec_init(0, MTree);
            find_mtree_h(childs+i, mtree_h, min_h);
            // Se insertan estos árboles a T'
            // Se insertan los puntos raíz de T'_1, . . .,T'_p, f1, . . . ,fp en F
            for(int i=0; i<vec_len(mtree_h); i++){
                MTree *mtree_t = (MTree*)vec_get(mtree_h, i);
                samplef_push(F, &(mtree_t->p));
            }
            samplef_assign_vector_strtpos(F, v, pos);

            vec_destroy(mtree_h);
        }
    }

    // Se define T_sup == mtree como el resultado de la llamada al algoritmo CP aplicado a F
    Vector *T_sup = samplef_get_sample(F);
    mtree = bulk_loading((Point const *)vec_to_array(T_sup), vec_len(T_sup));
    // cortar aquellos que sobran
    for(int i=0; i<mtree.n; i++){
        if(mtree.a[i].a != NULL){
            free(mtree.a[i].a);
        }
        mtree.a[i].a = NULL;
    }

    //Se une cada Tj ∈ T' a su hoja en T_sup correspondiente al punto pfj ∈ F, 
    // obteniendo un nuevo árbol T .
    MTree *t_prima = (MTree*)vec_to_array(T_prima);
    for(int i=0; i<vec_len(T_prima); i++){
        Point p = t_prima[i].p;
        double min_d2 = 1e5;
        int k = -1;

        // encontrar el nodo mas cercano
        for(int j=0; j<mtree.n; j++){
            double d2 = dist2(p, mtree.a[j].p);
            if(dist2(p, mtree.a[j].p) < min_d2 && mtree.a[j].a == NULL){
                min_d2 = d2;
                k = j;
            }
        }
        mtree.a[k] = t_prima[i];
    }

    //Se setean los radios cobertores resultantes para cada entrada en este árbol.
    // ademas la altura = h_min + 1
    update_radius(&mtree);

    // liberar memoria utilizada
    vec_destroy(T);
    vec_destroy(T_prima);
    samplef_destroy(F);
    return mtree;
}


MTree* mtree_create_cp(const std::vector<Point> &points, int n){
    MTree *mtree = (MTree*)malloc(sizeof(MTree));
    *mtree = bulk_loading(points);
    return mtree;
}


/** Agrega hijos a un nodo MTree */
static void add_childs(MTree *mtree, const std::vector<Point> &points){
    double max_d2 = 0;

    for(int i=0; i<points.size(); i++){
        double d2 = dist2(mtree->p, points[i]);
        max_d2 = d2 > max_d2 ? d2 : max_d2;
        (mtree->a)[i] = (MTree){points[i], 0, NULL, 0, 0};
    }
    mtree->cr = sqrt(max_d2); 
}

static void assign_to_nearest(std::vector<Point, std::vector<Point>> &F, std::vector<Point> &points){
    return;
}


/** Recorre el sample en busca de los F_k que tq |F_k| < b */
static void redistribution(SampleF *f){
    for(int i=0; i<samplef_len(f); i++){
        Vector *v = samplef_get_points(f, i);
        if(vec_len(v) < (B >> 1)){
            v = samplef_pop(f, i);
            samplef_assign_from_vector(f, v);
            vec_destroy(v);

            redistribution(f);
            break;
        }
    }
}


static void update_radius(MTree *mtree){
    double max_d2 = 0;
    int max_h = 0;
    for(int i=0; i<mtree->n; i++){
        double d2 = dist2(mtree->p, (mtree->a)[i].p);
        max_d2 = d2 > max_d2 ? d2 : max_d2;
        max_h = (mtree->a)[i].h > max_h ? (mtree->a)[i].h : max_h;
    }
    mtree->cr = sqrt(max_d2);
    mtree->h = max_h+1;
}

static void find_mtree_h(MTree *mtree, Vector *v, int h){
    if(mtree == NULL)
        return;

    if(mtree->h == h){
        vec_push(v, mtree);
        return;
    }

    for(int i=0; i<mtree->n; i++){
        find_mtree_h(mtree->a+i, v, h);
    }
}