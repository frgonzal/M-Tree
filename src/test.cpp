#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <queue>
#include "../headers/point.hpp"
#include "../headers/mtree.hpp"
#include "../headers/utils/random.hpp"
#include "../headers/utils/closest_pair.hpp"
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
    fileName << "./resultados/cp/mtree_" << power << ".csv";
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


int main(int argc, char **argv){
    printf("\t=====  TEST  =====\n");

    for(int k=0; k< 1e3; k++){

        std::vector<Point> points = random_sample_generator((35), k);
        //printf_vector(points);

        int i, j;
        std::tie(i, j) = closest(points);


        double min_dist = 1e9;
        int n1, n2;

        for(int i=0; i<points.size(); i++){
            for(int j=0; j<points.size(); j++){
                if(i == j) 
                    continue;

                double d2 = dist2(points[i], points[j]);
                if(d2 < min_dist){
                    min_dist = d2;
                    n1 = i;
                    n2 = j;
                }
            }
        }
        if(!((n1 == j && n2 == i) || (n1 == i && n2 == j))){
            printf("=======================");
            std::cout << "\n" << i << " " << j << "\n";
            std::cout << dist(points[i], points[j]) << "\n";
            std::cout << "\n" << n1 << " " << n2 << "\n";
            std::cout << dist(points[n1], points[n2]) << "\n";
            printf("=======================");
        }
    }

}