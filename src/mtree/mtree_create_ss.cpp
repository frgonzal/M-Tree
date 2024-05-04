#include <stdio.h>
#include <vector>
#include "../../headers/mtree.hpp"
#include "../../headers/point.hpp"

#define INFINITY 1e9;

static void outout_hoja();

MTree* mtree_create_ss(const std::vector<Point> &points){
    if (points.size() <= B) {
        //std::tuple<, double, MTree*>
        //return OutputHoja(&cluster);
        return nullptr;
    }

  Vector *clusters = Cluster(points, b, B);
  Vector *nodes = vec_init_(vec_len(clusters), sizeof(MTree *));
  // Llenar 'nodes' con nodos de hoja para cada cluster, no se tampoco

  while (vec_len(nodes) > B) {
    Vector *newClusters = Cluster(/* extraer puntos de nodos */, b, B);
    Vector *newNodes = vec_init_(vec_len(newClusters), sizeof(MTree *));
    // Llenar los 'newNodes' con nodos internos, no se como chucha aun
    vec_destroy(nodes);
    nodes = newNodes;
  }

  MTree *root = OutputInterno(nodes);
  vec_destroy(nodes);
  return root;
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

MTree *OutputHoja(Cluster *cluster) {
  MTree *node = malloc(sizeof(MTree));
  *node = (MTree){cluster->medoid, 0, NULL, 0, 0};
  return node;
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

int cluster_dist(Vector *cluster1, Vector *cluster2) {
  Point medoid1 = medoid_of_cluster(cluster1);
  Point medoid2 = medoid_of_cluster(cluster2);
  return dist(medoid1, medoid2);
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

Point medoid_of_cluster(Vector *cluster) {
  Point bestMedoid;
  double minDistanceSum = DBL_MAX;
  // Recorrer cada punto en el cluster y calcular la suma de distancias a todos los otros puntos
  for (int i = 0; i < vec_len(cluster); i++) {
    Point *currentPoint = (Point *)vec_get(cluster, i);
    double distanceSum = 0;
    for (int j = 0; j < vec_len(cluster); j++) {
      if (i != j) {
        Point *otherPoint = (Point *)vec_get(cluster, j);
        distanceSum += dist(*currentPoint, *otherPoint);
      }
    }
    if (distanceSum < minDistanceSum) {
      minDistanceSum = distanceSum;
      bestMedoid = *currentPoint;
    }
  }
  return bestMedoid;
}