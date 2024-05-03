#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <queue>
#include "../headers/point.hpp"
#include "../headers/mtree.hpp"
#include "../headers/utils/random.hpp"


//#define MAX 33554432
#define MAX 33554


// Function to calculate elapsed time in milliseconds
long long elapsed_time_millis(struct timespec *start, struct timespec *end) {
    long long start_ms = start->tv_sec * 1000LL + start->tv_nsec / 1000000LL;
    long long end_ms = end->tv_sec * 1000LL + end->tv_nsec / 1000000LL;
    return end_ms - start_ms;
}


static void printf_vector(const std::vector<Point> &points){
    printf("[");
    for(int i=0; i<points.size(); i++){
        printf("(%.2f, %.2f)",points[i].x, points[i].y);
        if(i < points.size()-1)
            printf(", ");
    }
    printf("]\n");
}

static void printf_mtree(MTree *raiz){
    std::queue<MTree*> q;
    std::vector<std::vector<MTree*>> niveles(raiz->h+1);
    q.push(raiz);

    while(!q.empty()){
        MTree* mtree = q.front();
        q.pop();
        niveles[mtree->h].push_back(mtree);

        for(int i=0; i<mtree->a.size(); i++){
            q.push(&mtree->a[i]);
        }
    }

    for(int i=niveles.size()-1; i>=0; i--){
        printf("\t-- Nivel %d --\n", i);
        for(int j=0; j<niveles[i].size(); j++){

            MTree *mtree = niveles[i][j];
            printf("p:(%.2f, %.2f),h:%d,n:%ld,cr:%1.2f;\n",mtree->p.x, mtree->p.y, mtree->h, mtree->a.size(), mtree->cr);
        }
    }
}


void random_test(int n, int k, int m){
    struct timespec start, end;
    long long duration;

    printf("\n== random test ==\n");

    printf("random sample generator test\n");
    std::vector<Point> points = random_sample_generator(n);
    //printf_vector(points);
    printf("time: %lld\n", duration);

    printf("random k sample test\n");

    while(m--){
        std::vector<Point> points_k = random_k_sample(points, k);
        printf_vector(points_k);
    }
}


void mtree_cp_test(int n){
    printf("\n\tMetodo CP \n");
    MTree* mtree;
    std::vector<Point> points = random_sample_generator(n);

    printf("Creando MTree...\n");
    mtree = mtree_create_cp(points);

    printf("points:\n");
    printf_vector(points);

    printf(" MTree:\n");
    printf_mtree(mtree);

    printf("\n- Search for all elements\n");
    {
        Point q = {0.5, 0.5};
        double r = 0.5;
        std::tuple<std::vector<Point>, int> ms = mtree_search(mtree, q, r);
        printf("\nI/Os: %d\n", std::get<1>(ms));
        //printf_array();
    }

    delete mtree;
}


int main(){
    srand(time(NULL));
    printf("\t=====  TEST  =====\n");


    mtree_cp_test(1000);
}