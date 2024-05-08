#include <cstddef>
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

static void printf_mtree(MTree *mtree, int power){
    std::ostringstream fileName;
    fileName << "./resultados/cp/mtree_" << power << ".csv";
    std::ofstream outFile(fileName.str());

    std::queue<Node*> q;
    q.push(mtree->root);

    while(!q.empty()){
        Node *node = q.front();
        q.pop();

        if(node == nullptr)
            continue;

        int h = node->h;
        for(Entry e : node->entries){
            q.push(e.a);
            int n = (e.a == nullptr) ? 0 : e.a->entries.size();
            outFile << "p:(" << e.p.x 
                    << ","   << e.p.y 
                    <<"),h:" << h
                    <<",n:"  << n
                    <<",cr:" << e.cr
                    <<";\n";
        }
    }
    outFile.close();
}


int main(int argc, char **argv){
    printf("\t=====  TEST  =====\n");

}