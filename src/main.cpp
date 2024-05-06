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
#include <chrono>

static void printf_mtree(MTree *raiz, int power, std::string method){
    std::ostringstream fileName;
    fileName << "./resultados/" << method << "/mtree/result/" << power << ".csv";
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

void printf_time(double seconds, std::string method_type, std::string test_type, int power){
    std::ostringstream fileName;
    fileName << "./resultados/" << method_type << "/" << test_type << "/time/" << power << ".csv";
    std::ofstream outFile(fileName.str());
    outFile << seconds << "\n";
    outFile.close();
}

void mtree_test(int power, int queries, int seed_sample, int seed_query, double r, MTree*(*constructor)(const std::vector<Point>&), std::string type){
    int size = (1 << power);
    MTree* mtree;
    std::vector<Point> points = random_sample_generator(size, seed_sample);

    printf("\nCreating MTree of %d elements\n", size);

    auto start = std::chrono::high_resolution_clock::now();
    mtree = (*constructor)(points);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Time taken to create MTree: " << elapsed.count() << " seconds\n";
    printf_time(elapsed.count(), type, "mtree", power);

    if(size < 17) printf_mtree(mtree, power, type);



    std::ostringstream fileName;
    fileName << "./resultados/" << type << "/search/result/" << power << ".csv";
    std::ofstream outFile(fileName.str());

    double total_time = 0;
    std::vector<Point> query_points = random_sample_generator(queries, seed_query);
    for(int i=0; i<queries; i++){    
        Point q = query_points[i];

        start = std::chrono::high_resolution_clock::now();
        std::tuple<std::vector<Point>, int> ms = mtree_search(mtree, q, r);
        end = std::chrono::high_resolution_clock::now();
        elapsed = (end - start);
        total_time += elapsed.count();

        outFile << "size:"<<std::get<0>(ms).size() <<",ios:"<< std::get<1>(ms)<<";\n";
    }
    elapsed = end - start;
    std::cout << "Time taken for 100 queries: " << total_time << " seconds\n";
    printf_time(total_time, type, "search", power);

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

    printf("\nMetodo CP \n");
    for(int power=10; power<=25; power++)
        mtree_test(power, queries, seed_sample, seed_query, r, &mtree_create_cp, "cp");

    printf("\nMetodo SS \n");
    for(int power=10; power<=25; power++)
        mtree_test(power, queries, seed_sample, seed_query, r, &mtree_create_ss, "ss");
}