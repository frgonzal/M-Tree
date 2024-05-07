#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>
#include <queue>
#include "../headers/point.hpp"
#include "../headers/mtree.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>


static void printf_vector(const std::vector<Point> &points){
    printf("[");
    for(int i=0; i<points.size(); i++){
        printf("(%.3f, %.3f)",points[i].x, points[i].y);
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

}