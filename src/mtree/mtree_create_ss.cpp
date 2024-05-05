#include <cstddef>
#include <stdio.h>
#include <vector>
#include <tuple>
#include "../../headers/mtree.hpp"
#include "../../headers/point.hpp"

#define MAX 1e9;

typedef std::tuple<Point, double, MTree*> tupleGRA;


static tupleGRA output_hoja(const std::vector<Point> &cluster);

static tupleGRA output_interno(std::vector<tupleGRA> &tuplas);

static std::vector<std::vector<Point>> cluster(const std::vector<Point> &cluster);

static Point find_medoid(const std::vector<Point> &points);

static double cluster_dist(const std::vector<Point> &cluster1, const std::vector<Point> &cluster2);


MTree* mtree_create_ss(const std::vector<Point> &points){
    /* Si |Cin| ≤ B: Se define (g, r, a) = OutputHoja(Cin) y se retorna a */
    if (points.size() <= B) {
        std::tuple<Point, double, MTree*> t = output_hoja(points);
        return std::get<2>(t);
    }

    /* Sea C_out = Cluster(C_in). Sea C = {}. */
    std::vector<std::vector<Point>> C_out = cluster(points);
    std::vector<tupleGRA> C(C_out.size());

    /* Por cada c ∈ Cout: Se añade OutputHoja(c) a C */
    for(int i=0; i<C_out.size(); i++)
        C[i] = output_hoja(C_out[i]);

    /* Mientras |C| > B: */
    while(C.size() > B){
        /* Sea Cin = {g|(g, r, a) ∈ C}. Sea Cout = Cluster(Cin). Sea Cmra = {}.*/
        std::vector<Point> C_in(C.size());
        for(int i=0; i<C.size(); i++)
            C_in[i] = std::get<0>(C[i]); // C_in[i] = g

        C_out = cluster(C_in);

        std::vector<tupleGRA> C_mra;

        /* Por cada c ∈ Cout: Sea s = {(g, r, a)|(g, r, a) ∈ C ∧ g ∈ c}, se añade s a Cmra */
        for(int i=0; i<C_out.size(); i++){
            std::vector<Point> c = C_out[i];
            Point g = find_medoid(c);
            for(int j=0; j<C.size(); j++){
                if( point_equal(std::get<0>(C[j]), g))
                    C_mra.push_back(C[j]);
            }
        }
        C.clear();

        /* Por cada s ∈ Cmra: Añadir OutputInterno(s) a C */
        C.resize(C_mra.size());
        for(int i=0; i<C_mra.size(); i++){
            std::vector<tupleGRA> v = {C_mra[i]};
            C[i] = output_interno(v);
        }
    }

    /* Sea (g, r, a) = OutputInterno(C) */
    /* Se retorna a */
    return std::get<2>(output_interno(C));
}


/** El medoide de un cluster es un punto g en el cluster tal que no existe otro punto p en el cluster
*   que, al ser nominado como medoide, genere un radio menor al entregado por g 
*/
static Point find_medoid(const std::vector<Point>& points) {
    double min_sum = MAX;
    Point medoid;

    for (int i=0; i<points.size(); i++) {
        double sum = 0;
        for (int j=0; j<points.size(); j++)
            sum += dist(points[i], points[j]);

        if (sum < min_sum) {
            medoid = points[i];
            min_sum = sum;
        }
    }
    return medoid;
}


/** La distancia entre dos clusters es la distancia entre sus medoides primarios. */
static double cluster_dist(std::vector<Point> &cluster1, std::vector<Point> &cluster2) {
    Point medoid1 = find_medoid(cluster1);
    Point medoid2 = find_medoid(cluster2);
    return dist(medoid1, medoid2);
}

static tupleGRA output_hoja(const std::vector<Point> &cluster){
    /* Sea g el medoide primario de Cin. Sea r = 0. Sea C = {} (el que corresponderá al nodo hoja). */
    /* Por cada p ∈ Cin: Añadimos (p, null, null) a C. Seteamos r = max(r, dist(g, p)) */
    /* Guardamos el puntero a C como a. */
    /* Retornamos (g, r, a)*/
}

// Función para asignar cada punto a su centro de cluster (sample) más cercano
void assign_points_to_closest_sample(Point *points, int total_points, Point *samples, int total_samples, int *closest_sample_indices) {
    for (int point_index = 0; point_index < total_points; point_index++) {
        double smallest_distance = INFINITY; // Iniciar con la distancia más grande posible
        int closest_sample_index = 0; // Índice del sample más cercano

        // Iterar sobre cada sample para encontrar el más cercano al punto actual
        for (int sample_index = 0; sample_index < total_samples; sample_index++) {
            double current_distance = calculate_distance(points[point_index], samples[sample_index]);
            if (current_distance < smallest_distance) {
                smallest_distance = current_distance; // Actualizar la distancia más pequeña encontrada
                closest_sample_index = sample_index; // Actualizar el índice del sample más cercano
            }
        }

        // Asignar el índice del sample más cercano al arreglo de índices
        closest_sample_indices[point_index] = closest_sample_index;
    }
}


Vector *Cluster(Vector *points, int b, int B) {
    Vector *C = vec_init_(vec_len(points), sizeof(Vector *));
    Vector *C_Res = vec_init_(0, sizeof(Vector *)); 

    // 1: Crear clusters con un solo punto 
    for (int i = 0; i < vec_len(points); i++) {
        Point *p = (Point *)vec_get(points, i);
        Vector *singlePointCluster = vec_init_(1, sizeof(Point));
        vec_push(singlePointCluster, p);
        vec_push(C, &singlePointCluster); 
    }

    // 2: Combinar clusters
    while (vec_len(C) > 1) {
        int minDistance = INT_MAX;
        int bestPair[2] = {-1, -1};

        // par más cercano de clusters
        for (int i = 0; i < vec_len(C) - 1; i++) {
            for (int j = i + 1; j < vec_len(C); j++) {
                Vector *cluster1 = *(Vector **)vec_get(C, i);
                Vector *cluster2 = *(Vector **)vec_get(C, j);
                int dist = cluster_dist(cluster1, cluster2); 

                if (dist < minDistance && (vec_len(cluster1) + vec_len(cluster2) <= B)) {
                    minDistance = dist;
                    bestPair[0] = i;
                    bestPair[1] = j;
                }
            }
        }

        // Juntar los clusters más cercanos
        if (bestPair[0] != -1 && bestPair[1] != -1) {
            Vector *newCluster = combine_clusters(*(Vector **)vec_get(C, bestPair[0]), *(Vector **)vec_get(C, bestPair[1]));
            vec_remove(C, bestPair[1]); 
            vec_remove(C, bestPair[0]);
            vec_push(C, &newCluster);
        } else {
            // Si no se pueden combinar más clusters, mover todos los clusters restantes a C_Res ??????? Gracias GPAPITO, MEDIO ERROR ME ENCONTRASTE
            while (vec_len(C) > 0) {
                Vector *remainingCluster = *(Vector **)vec_pop(C, 0);
                vec_push(C_Res, &remainingCluster);
            }
        }
    }

    vec_destroy(C); 
    return C_Res;
}

MTree *OutputInterno(Vector *children) {
    if (vec_len(children) == 0)
        return NULL;

    Point medoid =
        /* función para calcular el medoide de los medoides de los hijos */;
    double max_radius = 0;
    MTree *node = malloc(sizeof(MTree));
    node->a = malloc(vec_len(children) * sizeof(MTree));

    for (int i = 0; i < vec_len(children); ++i) {
        MTree *child = (MTree *)vec_get(children, i);
        double distance = dist(medoid, child->p) + child->cr;
        max_radius = fmax(max_radius, distance);
    }

    *node = (MTree){medoid, max_radius, node->a, vec_len(children),
                  1 + ((MTree *)vec_get(children, 0))->h};
    return node;
}



Vector *combine_clusters(Vector *cluster1, Vector *cluster2) {
    Vector *newCluster =
        vec_init_(vec_len(cluster1) + vec_len(cluster2), sizeof(Point));
    for (int i = 0; i < vec_len(cluster1); i++) {
        Point *p = (Point *)vec_get(cluster1, i);
        vec_push(newCluster, p);
    }
    for (int j = 0; j < vec_len(cluster2); j++) {
        Point *p = (Point *)vec_get(cluster2, j);
        vec_push(newCluster, p);
    }
    return newCluster;
}