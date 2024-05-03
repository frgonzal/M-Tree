#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <queue>
#include "../headers/point.hpp"
#include "../headers/mtree.hpp"
#include "../headers/utils/random.hpp"

#include <fstream>
#include <iostream>
#include <sstream> // Include for ostringstream
#include <ctime>

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

static void printf_mtree(MTree *raiz, int size){
    std::ostringstream fileName;
    fileName << "./resultados/mtree_cp_" << size << ".csv";
    std::ofstream outFile(fileName.str());

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
        //printf("\t-- Nivel %d --\n", i);
        for(int j=0; j<niveles[i].size(); j++){

            MTree *mtree = niveles[i][j];
            //printf("p:(%.20f, %.20f),h:%d,n:%ld,cr:%1.20f;\n",mtree->p.x, mtree->p.y, mtree->h, mtree->a.size(), mtree->cr);
            outFile << "p:(" << mtree->p.x 
                    << ","   << mtree->p.y 
                    <<"),h:" << mtree->h
                    <<",n:"  << mtree->a.size()
                    <<",cr:" << mtree->cr
                    <<";\n";
        }
    }

    outFile.close();
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


void mtree_cp_test(int size, int querys){
    printf("\nMetodo CP \n");
    MTree* mtree;
    std::vector<Point> points = random_sample_generator(size);

    printf("Creando MTree de %d elementos\n", size);
    mtree = mtree_create_cp(points);
    printf_mtree(mtree, size);

    printf("\tMTree (h=%d):\n", mtree->h);

    printf("\n=== Search Query ===\n");
    while(querys--){
        Point q = random_point();
        double r = 0.02;
        std::tuple<std::vector<Point>, int> ms = mtree_search(mtree, q, r);
        printf("\tPoint: (%.5f, %.5f), I/Os: %d\n", q.x, q.y, std::get<1>(ms));
    }

    delete mtree;
}


int main(){
    srand(time(NULL));
    printf("\t=====  TEST  =====\n");

    int size = (1 << 10);
    mtree_cp_test(size, 1);
}