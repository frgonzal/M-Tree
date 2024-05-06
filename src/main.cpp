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

static void printf_mtree(MTree *raiz, int power, std::string type){
    std::ostringstream fileName;
    fileName << "./resultados/" << type << "/mtree_" << power << ".csv";
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
        for(int j=0; j<niveles[i].size(); j++){

            MTree *mtree = niveles[i][j];
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

void mtree_test(int power, int queries, int seed_sample, int seed_query, double r, MTree*(*constructor)(const std::vector<Point>&), std::string type){
    int size = (1 << power);
    MTree* mtree;
    std::vector<Point> points = random_sample_generator(size, seed_sample);

    printf("Creando MTree de %d elementos\n\n", size);
    mtree = (*constructor)(points);
    printf_mtree(mtree, power, type);

    std::ostringstream fileName;
    fileName << "./resultados/" << type << "/search_" << power << ".csv";
    std::ofstream outFile(fileName.str());

    std::vector<Point> query_points = random_sample_generator(queries, seed_query);
    for(int i=0; i<queries; i++){    
        Point q = query_points[i];
        std::tuple<std::vector<Point>, int> ms = mtree_search(mtree, q, r);
        outFile << "size:"<<std::get<0>(ms).size() <<",ios:"<< std::get<1>(ms)<<";\n";
    }
    outFile.close();

    delete mtree;
}

int main(int argc, char **argv){
    srand(time(NULL));
    printf("\t=====  RUN  =====\n");

    int seed_sample = 1;
    int seed_query  = 2;
    int queries = 100;
    double r = 0.02;

    /*
    printf("\nMetodo CP \n");
    for(int power=10; power<=25; power++){
        mtree_test(power, queries, seed_sample, seed_query, r, &mtree_create_cp, "cp");
    }
    */

    printf("\nMetodo SS \n");
    for(int power=10; power<=13; power++){
        mtree_test(power, queries, seed_sample, seed_query, r, &mtree_create_ss, "ss");
    }
}