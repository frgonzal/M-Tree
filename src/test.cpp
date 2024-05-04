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
#include <sstream>
#include <ctime>


static void printf_vector(const std::vector<Point> &points){
    printf("[");
    for(int i=0; i<points.size(); i++){
        printf("(%.2f, %.2f)",points[i].x, points[i].y);
        if(i < points.size()-1)
            printf(", ");
    }
    printf("]\n");
}

static void printf_mtree(MTree *raiz, int power){
    std::ostringstream fileName;
    fileName << "./resultados/mtree_cp_" << power << ".csv";
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

void mtree_cp_test(int power, int queries, int seed_sample, int seed_query, double r){
    int size = (1 << power);
    MTree* mtree;
    std::vector<Point> points = random_sample_generator(size, seed_sample);

    printf("Creando MTree de %d elementos\n", size);
    mtree = mtree_create_cp(points);
    //printf_mtree(mtree, power);

    printf("Search Query \n");
    std::ostringstream fileName;
    fileName << "./resultados/cp/search_" << power << ".csv";
    std::ofstream outFile(fileName.str());

    std::vector<Point> query_points = random_sample_generator(queries, seed_query);
    for(int i=0; i<queries; i++){
        Point q = query_points[i];
        std::tuple<std::vector<Point>, int> ms = mtree_search(mtree, q, r);
        outFile << "ios:"<< std::get<1>(ms)<<"\n";
    }
    outFile.close();

    delete mtree;
}

void mtree_ss_test(int power, int queries, int seed_sample, int seed_query, double r){
    int size = (1 << power);
    MTree* mtree;
    std::vector<Point> points = random_sample_generator(size, seed_sample);

    printf("Creando MTree de %d elementos\n", size);
    mtree = mtree_create_ss(points);

    printf("Search Query \n");
    std::ostringstream fileName;
    fileName << "./resultados/ss/search_" << power << ".csv";
    std::ofstream outFile(fileName.str());

    std::vector<Point> query_points = random_sample_generator(queries, seed_query);
    for(int i=0; i<queries; i++){    
        Point q = query_points[i];
        std::tuple<std::vector<Point>, int> ms = mtree_search(mtree, q, r);
        outFile << "ios:"<< std::get<1>(ms)<<"\n";
    }
    outFile.close();

    delete mtree;
}


int main(){
    srand(time(NULL));
    printf("\t=====  TEST  =====\n");

    int seed_sample = 10;
    int seed_query  = 20;
    int queries = 100;
    double r = 0.02;

    printf("\nMetodo CP \n");
    for(int power=10; power<=25; power++){
        mtree_cp_test(power, queries, seed_sample, seed_query, r);
    }
}