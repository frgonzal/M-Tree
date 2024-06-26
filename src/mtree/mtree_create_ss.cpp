#include <cstdlib>
#include <cwchar>
#include <vector>
#include <tuple>
#include <algorithm>
#include "../../headers/mtree.hpp"
#include "../../headers/point.hpp"
#include "../../headers/utils/closest_pair.hpp"

#define MAX 1e9;

typedef std::vector<Point> Cluster;


/** The algorithm to create the MTree */
Node* ss_algorithm(const std::vector<Point> &points);

/** The OutputHoja algorithm*/
static Entry output_hoja(const Cluster &cluster);

/** The OutputInterno algorithm */
static Entry output_interno(std::vector<Entry> &mtrees);

/** The algorithm to create clusters. Returns a vector of clusters */
static std::vector<Cluster> create_clusters(const Cluster &cluster);

/** Returns the medoid of a cluster */
static Point find_medoid(const Cluster &points);

/** Find the neares cluster to another cluster. it uses the medoid instead of the cluster */
static Cluster closest_neighbor(Point g, std::vector<Cluster> &clusters, Cluster &G);

/** Find the closest clusters from a vector of clusters, and returns them. */
static std::tuple<Cluster, Cluster> closest_clusters(std::vector<Cluster> &C, Cluster &G);

/** Create two clusters using the MinMaxSplitPolicy */
static std::tuple<Cluster, Cluster> min_max_split_policy(Cluster &cluster);

/** Returns the position of the searched point, using binary search */
static int point_bin_search(std::vector<Entry> points, Point p);



MTree* mtree_create_ss(const std::vector<Point> &points){
    Node *root = ss_algorithm(points);
    return new MTree(root);
}


Node* ss_algorithm(const std::vector<Point> &points){
    /* Si |Cin| ≤ B: Se define (g, r, a) = OutputHoja(Cin) y se retorna a */
    if (points.size() <= B) {
        Entry e = output_hoja(points);
        return e.a;
    }

    /* Sea C_out = Cluster(C_in). Sea C = {}. */
    std::vector<Cluster> C_out = create_clusters(points);
    std::vector<Entry> C(C_out.size());

    /* Por cada c ∈ Cout: Se añade OutputHoja(c) a C */
    for(int i=0; i<C_out.size(); i++)
        C[i] = output_hoja(C_out[i]);

    /* Mientras |C| > B: */
    while(C.size() > B){
        /* Sea Cin = {g|(g, r, a) ∈ C} */
        std::vector<Point> C_in(C.size());
        for(int i=0; i<C.size(); i++)
            C_in[i] = C[i].p;

        /* Sea Cout = Cluster(Cin).*/
        C_out = create_clusters(C_in);

        /* Sea Cmra = {}.*/
        std::vector<std::vector<Entry>> C_mra(C_out.size());

        std::sort(C.begin(), C.end(), [](Entry e1, Entry e2){
            return  (e1.p < e2.p);
        });

        for(int i=0; i<C_out.size(); i++){
            /* Por cada c ∈ Cout */
            std::vector<Point> c = C_out[i];

            /* Sea s = {(g, r, a)|(g, r, a) ∈ C ∧ g ∈ c} */ 
            std::vector<Entry> s(c.size());
            for(int j=0; j<c.size(); j++){
                int k = point_bin_search(C, c[j]);
                s[j] = C[k];
            }
            /* se añade s a Cmra */
            C_mra[i] = s;
        }
        C.clear();

        /* Por cada s ∈ Cmra: Añadir OutputInterno(s) a C */
        C.resize(C_mra.size());
        for(int i=0; i<C_mra.size(); i++){
            C[i] = output_interno(C_mra[i]);
        }
    }

    /* Sea (g, r, a) = OutputInterno(C). Se retorna a */
    Entry e = output_interno(C);
    return e.a;
}


static std::vector<Cluster> create_clusters(const std::vector<Point> &cluster){
    /* Se define Cout = {} y C = {}*/
    std::vector<Cluster> C_out;
    Cluster G_out;

    std::vector<Cluster> C(cluster.size());
    Cluster G(cluster.size());

    /* Por cada punto p ∈ Cin se añade {p} a C.*/
    for(int i=0; i<cluster.size(); i++){
        C[i] = std::vector({cluster[i]});
        G[i] = cluster[i];
    }


    /* Mientras |C| > 1:*/
    while(C.size() > 1){
        if(C.size() != G.size()) exit(1);
        if(C_out.size() != G_out.size()) exit(1);

        /* Sea c1, c2 los pares más cercanos de clusters en C tal que |c1| ≥ |c2|.*/
        Cluster c1, c2;
        std::tie(c1, c2) = closest_clusters(C, G);

        /* Si |c1 ∪ c2| ≤ B, se remueve c1 y c2 de C y se añade c1 ∪ c2 a C.*/
        if(c1.size() + c2.size() <= B){
            //merge clusters
            c1.insert(c1.end(), c2.begin(), c2.end());

            G.push_back(find_medoid(c1));
            C.push_back(std::move(c1));
        }else{ /* Si no, se remueve c1 de C y se añade c1 a Cout.*/
            G.push_back(find_medoid(c2));
            C.push_back(std::move(c2));

            G_out.push_back(find_medoid(c1));
            C_out.push_back(std::move(c1));
        }
    }
    /* Sea c el último elemento de C */

    Point g = G[0];
    std::vector<Point> c = std::move(C[0]);
    std::vector<Point> c_prima;
    /* Si |Cout| > 0: */
    if(C_out.size() > 0){
        /* definimos c' como el vecino más cercano a c en Cout. Removemos c' de Cout */
        c_prima = closest_neighbor(g, C_out, G_out);
    }else{ /* Si no, se define c' = {}. */
        c_prima = {};
    }

    /* Si |c ∪ c'| ≤ B: */
    c.insert(c.end(), c_prima.begin(), c_prima.end());
    if(c.size() <= B){
        /* Añadimos c ∪ c' a Cout. */
        C_out.push_back(std::move(c));
    }else{ 
        /* Si no, dividimos c ∪ c' en c1 y c2 usando MinMax split policy. */
        std::vector<Point> c1, c2;
        std::tie(c1, c2) = min_max_split_policy(c);

        /* Se añaden c1 y c2 a Cout.*/
        C_out.push_back(std::move(c1));
        C_out.push_back(std::move(c2));
    }
    
    /* Se retorna Cout */
    return C_out;
}


static Entry output_hoja(const std::vector<Point> &cluster){
    /* Sea g el medoide primario de Cin. Sea r = 0. Sea C = {} (el que corresponderá al nodo hoja). */
    Point g = find_medoid(cluster);

    Node *node = new Node();
    Entry entry = Entry(g);
    entry.a = node;

    /* Por cada p ∈ Cin: Añadimos (p, null, null) a C. Seteamos r = max(r, dist(g, p)) */
    for(const Point point : cluster){
        Entry e = Entry(point);
        entry.a->entries.push_back(e);
        double d2 = dist(entry.p, point);
        entry.cr = std::max(entry.cr, d2);
    }
    /* Guardamos el puntero a C como a. */
    /* Retornamos (g, r, a)*/
    return entry;
}


static Entry output_interno(std::vector<Entry> &C_mra){
    /*  Sea Cin = {g|∃(g, r, a) ∈ Cmra}. */
    std::vector<Point> C_in(C_mra.size());
    for(int i=0; i<C_mra.size(); i++)
        C_in[i] = C_mra[i].p;

    /* G el medoide primario de Cin. Sea R = 0. Sea C = {}.*/
    Point g = find_medoid(C_in);

    Node *node = new Node();
    Entry entry = Entry(g);
    entry.a = node;

    /* Por cada (g, r, a) ∈ Cmra: Añadir (g, r, a) a C. Se setea R = max(R, dist(G, g) + r) */
    for(int i=0; i<C_mra.size(); i++){
        entry.a->entries.push_back((C_mra[i]));

        double d = dist(entry.p, C_mra[i].p) + C_mra[i].cr;
        entry.cr = std::max(entry.cr, d);

        for(Entry entry_child : entry.a->entries){
            if(entry_child.a != nullptr)
                entry.a->h = entry_child.a->h >= entry.a->h ? entry_child.a->h + 1 : entry.a->h;
        }
    }

    /* Guardamos el puntero a C como A.*/
    /* Retornamos (G, R, A)*/
    return entry;
}


/** El medoide de un cluster es un punto g en el cluster tal que no existe otro punto p en el cluster
*   que, al ser nominado como medoide, genere un radio menor al entregado por g 
*/
static Point find_medoid(const Cluster &points) {
    double min_r = MAX;
    Point medoid;

    for (Point p1 : points) {
        double max_d2 = 0;
        for (Point p2 : points){
            double d2 = dist2(p1, p2);
            max_d2 = max_d2 > d2 ? max_d2 : d2;

            if(!(max_d2 < min_r)) break;
        }

        if (max_d2 < min_r) {
            min_r = max_d2;
            medoid = p1;
        }
    }
    return medoid;
}

/** Busca el vecino mas cercano, lo elmina de clusters y lo retorna */
static Cluster closest_neighbor(Point g, std::vector<Cluster> &clusters, Cluster &G){
    double min_d2 = MAX;
    int pos = -1;

    for(int i=0; i<clusters.size(); i++){
        double d2 = dist2(g, G[i]);
        if(d2 < min_d2){
            min_d2 = d2;
            pos = i;
        }
    }

    Cluster res = std::move(clusters[pos]);

    clusters[pos] = std::move(clusters[clusters.size()-1]);
    clusters.pop_back();

    G[pos] = std::move(G[G.size()-1]);
    G.pop_back();

    return res;
}

/** Busca los 2 clusters mas cercanos, los elimina y los devuelve */
static std::tuple<Cluster, Cluster> closest_clusters(std::vector<Cluster> &C, Cluster &G){
    if(C.size() != G.size())
        exit(1);

    double min_dist = MAX;

    int n1, n2;
    std::tie(n1, n2) = closest(G);

    if(C[n1].size() < C[n2].size())
        std::swap(n1, n2);

    if(n2 == C.size()-1){
        if(n1 == C.size()-2){
            std::swap(C[n1], C[n2]);
            std::swap(G[n1], G[n2]);
            n1 = C.size()-1;
            n2 = C.size()-2;
        }else{
            std::swap(C[n2], C[C.size()-2]);
            std::swap(G[n2], G[G.size()-2]);
            n2 = C.size()-2;
        }
    }


    Cluster c1 = std::move(C[n1]);

    C[n1] = std::move(C[C.size()-1]);
    C.pop_back();

    G[n1] = std::move(G[G.size()-1]);
    G.pop_back();

    Cluster c2 = std::move(C[n2]);

    C[n2] = std::move(C[C.size()-1]);
    C.pop_back();

    G[n2] = std::move(G[G.size()-1]);
    G.pop_back();

    return std::make_tuple(c1, c2);
}


/** Devuelve la posicion del punto mas cercano del cluster al centro p */
static std::tuple<int, double> find_nearest_point(Point p, const Cluster &c){
    double distance2 = MAX;
    int pos = -1;

    for(int i=0; i<c.size(); i++){
        double d2 = dist2(p, c[i]);
        if(d2 < distance2){
            pos = i;
            distance2 = d2;
        }
    }
    return std::make_tuple(pos, distance2);
}

/** Assigna los puntos y retorna radio cobertor maximo al cuadrado */
static double assign_points(Point p1, Point p2, Cluster &c1, Cluster &c2, Cluster cluster, double best){
    double max_cover_radius2 = 0;

    int turn = 1;
    while(cluster.size() > 0){
        Point p;
        Cluster *c;

        if(turn){
            p = p1;
            c = &c1;
            turn = !turn;
        } else {
            p = p2;
            c = &c2;
            turn = !turn;
        }

        double d2;
        int pos; 
        std::tie(pos, d2) = find_nearest_point(p, cluster);

        (*c).push_back(cluster[pos]);
        max_cover_radius2 = d2 > max_cover_radius2 ? d2 : max_cover_radius2;

        if(!(max_cover_radius2 < best))// no vale la pena continuar
            return max_cover_radius2;

        std::swap(cluster[pos], cluster[cluster.size()-1]);
        cluster.pop_back(); 
    }

    return max_cover_radius2;
}

static std::tuple<Cluster, Cluster> min_max_split_policy(Cluster &cluster){
    Cluster c1_out, c2_out;
    c1_out.reserve(cluster.size()/2);
    c2_out.reserve(cluster.size()/2);

    double min_max_cover_radius2 = MAX;
    for(int i=0; i<cluster.size(); i++){
        for(int j=i+1; j<cluster.size(); j++){
            Cluster c1, c2;

            double max_cover_radius2 = assign_points(cluster[i], cluster[j], c1, c2, cluster, min_max_cover_radius2);
            if(max_cover_radius2 < min_max_cover_radius2){
                min_max_cover_radius2 = max_cover_radius2;
                c1_out = std::move(c1);
                c2_out = std::move(c2);
            }
        }
    }

    return std::make_tuple(c1_out, c2_out);
}

static int point_bin_search(std::vector<Entry> points, Point p){
    int l = 0, r = points.size(), mid; 
    while(l < r){
        mid = l+(r-l)/2;
        if(p <= points[mid].p) r = mid;
        else l = mid+1;
    }
    if(!(p <= points[l].p)) exit(2);
    else return l;
}