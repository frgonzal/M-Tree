#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <tuple>
#include <vector>
#include <queue>
#include <iostream>
#include "../../headers/mtree.hpp"
#include "../../headers/point.hpp"
#include "../../headers/utils/random.hpp"

long long abc = 0;

struct NodePoint{
    Node *node;
    const Point &p;
};

static void assign_to_nearest(std::vector<Point> &f, std::vector<std::vector<Point>> &F, const std::vector<Point> &points);

static void redistribution(std::vector<Point> &f, std::vector<std::vector<Point>> &F);

static std::vector<std::tuple<Node*, Point>> find_node_h(Node *node, int h);

static void append_to_leaf(std::vector<Entry*> &entries, const std::vector<Node*> &F, const std::vector<Point> &f);

static void update_radius(std::vector<Entry*> &entries);

static void update_height(std::vector<std::vector<Node*>> &levels);

static std::vector<Entry*> get_all_entries(Node *root);

static std::vector<Entry*> get_all_leaf_entries(Node *root);

static std::vector<std::vector<Node*>> get_all_levels(Node *root);


/** The bulk_loading algorithm */
Node* bulk_loading(const std::vector<Point> &points){
    abc++;
    // Si |P| ≤ B, se crea un árbol T , se insertan todos los puntos a T y se retorna T
    if(points.size() <= B){
        Node *node = new Node();
        for(const Point p : points){
            node->entries.push_back(Entry(p));
        }
        return node;
    }

    std::vector<Point> f;
    std::vector<std::vector<Point>> F;

    int k;
    for(;;){
        k = std::min(B, (int)std::ceil(((double)points.size())/B));

        // De manera aleatoria se eligen k = min(B, n/B) puntos de P, que los llamaremos samples pf1 , . . . , pfk . 
        // Se insertan en un conjunto F de samples.
        f = random_k_sample(points, k);

        // Se le asigna a cada punto en P su sample más cercano. Con eso se puede construir k conjuntos
        // F1, . . . , Fk 
        //std::cout<< "-==============\n";
        assign_to_nearest(f, F, points);

        // Etapa de redistribución: Si algún Fj es tal que |Fj| < b:
        redistribution(f, F);

        k = f.size();

        if(k > 1) 
            break;

        f.clear();
        F.clear();
    }

    std::vector<Node*> T;
    std::vector<Point> f2;
    // Se realiza recursivamente el algoritmo CP en cada Fj, obteniendo el árbol T
    for (int i=0; i<f.size(); i++){
        Node* root = bulk_loading(F[i]);
        if(root->entries.size() >= (B >> 1)){
            T.push_back(root);
            f2.push_back(f[i]);
        }else{ 
            // Si la raíz del árbol es de un tamaño menor a b
            // Se quita esa raíz se elimina pfj de F.
            // Se trabaja con sus subárboles como nuevos 
            // Tj , . . . , Tj+p−1, se añaden los puntos pertinentes a F.
            for(Entry &entry : root->entries){
                T.push_back(entry.a);
                f2.push_back(entry.p);
                entry.a = nullptr;
            }
            delete root;
        }
    }
    F.clear();
    F.shrink_to_fit();
    f = std::move(f2);

    // Etapa de balanceamiento: Se define h como la altura mínima de los árboles Tj. 
    int min_h = 1e8;
    // buscar altura minima entre los hijos
    for(int i=0; i<T.size(); i++)
        min_h = T[i]->h < min_h ? T[i]->h : min_h;
    
    // Se define T' inicialmente como un conjunto vacío.
    std::vector<Node*> T_prima(0);
    f2 = std::vector<Point>(0);

    //Por cada Tj , si su altura es igual a h, se añade a T'
    for(int i=0; i<T.size(); i++){
        if(T[i]->h == min_h){
            T_prima.push_back(T[i]);
            f2.push_back(f[i]);
        }else{ //Si no se cumple:
            //Se borra el punto pertinente en F.
            //Se hace una búsqueda exhaustiva en Tj de todos los subárboles T'_1, . . . ,T'_p 
            // de altura igual a h. 
            
            std::vector<std::tuple<Node*, Point>> nodes = find_node_h(T[i], min_h);
            // Se insertan estos árboles a T'
            // Se insertan los puntos raíz de T'_1, . . .,T'_p, f1, . . . ,fp en F
            for(auto[node, point] : nodes){
                T_prima.push_back(node);
                f2.push_back(point);
            }
            delete T[i];
        }
    }
    T.clear();
    T.shrink_to_fit();
    f = std::move(f2);

    // Se define T_sup == mtree como el resultado de la llamada al algoritmo CP aplicado a F
    Node *root = bulk_loading(f);
    std::vector<std::vector<Node*>> T_sup_levels = get_all_levels(root);
    std::vector<Entry*> T_sup_entries = get_all_entries(root);
    std::vector<Entry*> T_sup_leaf_entries = get_all_leaf_entries(root);

    //Se une cada Tj ∈ T' a su hoja en T_sup correspondiente al punto pfj ∈ F, 
    // obteniendo un nuevo árbol T .
    append_to_leaf(T_sup_leaf_entries, T_prima, f);

    //Se setean los radios cobertores resultantes para cada entrada en este árbol.
    update_radius(T_sup_entries);
    update_height(T_sup_levels);

    return root;
}


MTree* mtree_create_cp(const std::vector<Point> &points){
    Node *root = bulk_loading(points);
    return new MTree(root);
}


/** Assigns points to their neares sample */
static void assign_to_nearest(std::vector<Point> &f, std::vector<std::vector<Point>> &F, const std::vector<Point> &points){
    F.resize(f.size(), std::vector<Point>(0));
    for(int i=0; i<points.size(); i++){
        int k = -1;
        double min_d2 = 1e5;
        for(int j=0; j<f.size(); j++){
            double d2 = dist2(points[i], f[j]);
            if(d2 < min_d2){
                min_d2 = d2;
                k = j;
            }
        }
        F[k].push_back(points[i]);
    }
}


/** Recorre el sample en busca de los F_k que tq |F_k| < b */
static void redistribution(std::vector<Point> &f, std::vector<std::vector<Point>> &F){
    int i=0;
    while(i < f.size()){
        if(F[i].size() < (B >> 1)){
            std::vector<Point> points = F[i];

            std::swap(f[i], f[f.size()-1]);
            std::swap(F[i], F[f.size()-1]);
            f.pop_back();
            F.pop_back();

            assign_to_nearest(f, F, points);
        }else
            i++;
    }
}


/** Returns a vector with all the sub Trees that have height == h*/
static std::vector<std::tuple<Node*, Point>> find_node_h(Node *node, int h){
    std::vector<std::tuple<Node*, Point>> nodes_p;
    std::queue<std::tuple<Node*, Point*>> q;
    q.push(std::make_tuple(node, nullptr));

    while(!q.empty()){
        auto[node, point] = q.front();
        q.pop();

        if(node == nullptr)
            continue;

        if(node->h == h){
            nodes_p.push_back(std::make_tuple(node, *point));
            continue;
        }

        for(Entry &e : node->entries){
            q.push(std::make_tuple(e.a, &e.p));
            e.a = nullptr;
        }
    }

    return nodes_p;
}

// retorna la ultima posicion donde la condicion p < points[mid]->p es falsa
static int bin_search(const std::vector<Entry*> &points, const Point &p){
    int l = 0, r = points.size()-1, mid; 
    while(l < r){
        mid = l+(r-l)/2;

        if(p <= points[mid]->p){
            r = mid;
        } 
        else {
            l = mid+1;
        }
    }

    if(!(p <= points[l]->p)) exit(11);
    return l;
}

static void append_to_leaf(std::vector<Entry*> &entries, const std::vector<Node*> &T, const std::vector<Point> &f){
    std::sort(entries.begin(), entries.end(), [](Entry *e1, Entry *e2){
        return e1->p < e2->p;
    });

    for(int i=0; i<T.size(); i++){
        int k = bin_search(entries, f[i]); 
        entries[k]->a = T[i];
    }

}


static double get_radius(Entry *entry){
    double max_d2 = 0;
    std::queue<Entry*> q;
    q.push(entry);

    while(!q.empty()){
        Entry *child = q.front();
        q.pop();

        if(child->a == nullptr){// is on a leaf
            double d2 = dist2(child->p, entry->p);
            max_d2 = d2 > max_d2 ? d2 : max_d2;
            continue;
        }

        for(Entry &e : child->a->entries){
            q.push(&e);
        }
    }   
    return std::sqrt(max_d2);
}


/** Updated radius and height of an mtree */
static void update_radius(std::vector<Entry*> &entries){
    for(Entry *e : entries){
        e->cr = get_radius(e);
    }    
}

static void update_height(std::vector<std::vector<Node*>> &levels){
    for(int i=0; i<levels.size(); i++){
        for(Node *node : levels[i]){
            for(Entry &e : node->entries){
                if(e.a != nullptr)
                    node->h = e.a->h >= node->h ? e.a->h+1 : node->h;
            }
        }
    }
}



static std::vector<Entry*> get_all_entries(Node *root){
    std::vector<Entry*> entries;
    std::queue<Node*> q;
    q.push(root);
    while(!q.empty()){
        Node *node = q.front();
        q.pop();

        if(node == nullptr)
            continue;

        for(Entry &e : node->entries){
            entries.push_back(&e);
            q.push(e.a);
        }
    }
    return entries;
}

static std::vector<Entry*> get_all_leaf_entries(Node *root){
    std::vector<Entry*> entries;
    std::queue<Node*> q;
    q.push(root);
    while(!q.empty()){
        Node *node = q.front();
        q.pop();

        if(node == nullptr)
            continue;

        for(Entry &e : node->entries){
            if(e.a == nullptr)
                entries.push_back(&e);
            else
                q.push(e.a);
        }
    }
    return entries;   
}


static std::vector<std::vector<Node*>> get_all_levels(Node *root){
    std::vector<std::vector<Node*>> levels;
    levels.resize(root->h+1);

    std::queue<Node*> q;
    q.push(root);
    while(!q.empty()){
        Node *node = q.front();
        q.pop();
        if(node == nullptr)
            continue;

        levels[node->h].push_back(node);

        for(Entry e: node->entries)
            q.push(e.a);
    }
    return levels;
}