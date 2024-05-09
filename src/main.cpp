#include <cstdlib>
#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <queue>
#include "../headers/point.hpp"
#include "../headers/mtree.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <ctime>
#include "../headers/utils/random.hpp"


/** Writes the structure of the MTree into a file 
*   Writes every Entry as [p, h, n, cr]
*   where h is the height of the node
*   and n is the number entries in the Node a
*/
static void printf_mtree(MTree *mtree, int power, std::string method){
    std::ostringstream fileName;
    fileName << "./resultados/" << method << "/mtree/result/" << power << ".csv";
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
            int n = (e.a == nullptr) ? 0 : e.a->entries.size();

            outFile << "p:(" << e.p.x 
                    << ","   << e.p.y 
                    <<"),h:" << h
                    <<",n:"  << n
                    <<",cr:" << e.cr
                    <<";\n";

            q.push(e.a);
        }
    }
    outFile.close();
}


int main(int argc, char **argv){
    srand(time(NULL));
    printf("\t=====  RUN  =====\n");

    int seed_sample = 42123423;
    int seed_query  = 51221343;
    int queries = 1000;
    double r = 0.02;

    int size = (2 << 10);

    // Generate the samples for the MTree and for the query
    std::vector<Point> points = random_sample_generator(size, seed_sample);
    std::vector<Point> query_points = random_sample_generator(queries, seed_query);

    MTree *mtree;

    std::cout << "Creando MTree con método CP\n";
    mtree = mtree_create_cp(points);
    double total_ios = 0;
    std::cout << "Realizando busquedas\n";
    for(int i=0; i<queries; i++){
        auto[_, ios] = mtree_search(mtree, query_points[i], r);
        total_ios += ios / (double) queries;
    }
    std::cout << "Promedio de I/O's por busqueda: " << total_ios << "\n\n";
    delete mtree;

    std::cout << "Creando MTree con método SS\n";
    mtree = mtree_create_ss(points);
    std::cout << "Realizando busquedas\n";
    total_ios = 0;
    for(int i=0; i<queries; i++){
        auto[_, ios] = mtree_search(mtree, query_points[i], r);
        total_ios += ios / (double) queries;
    }
    std::cout << "Promedio de I/O's por busqueda: " << total_ios << "\n\n";
    delete mtree;
}