// My NuMVC test source file
//    MuMVC, a heuristic algorithm to solve the Minimum Vertex Cover
// Problem, raised by Cai Shaowei, 2013 in paper `NuMVC: An Efficient
//  Local Search Algorithm for Minimum Vertex Cover`, url as :
// https://arxiv.org/abs/1402.0584.
// Re-implemented by Zachary-ZS.

#include <cmath>
#include <stack>
#include <iostream>
#include <set>
#include <string.h>

using namespace std;

/* parameters of the algorithm */
#define MAXV 780
#define MAXE 254000
typedef set<int> Set;

int V, E;           // |V| & |E| in this case

// To be set
int cutoff_time;    // Stop when time reaches cutoff
int threshold;      // Delicate w when \bar{w} reaches threshold
                    // NOTICE: Cai set it to 0.5*|V|
int conf_change[MAXV];
int dscore[MAXV];
int G[MAXV][MAXV] = {0};  // Graph
int edge_weight[MAXE] = {0};
int timer_mod[MAXV]; // Modified time, for vertex choosing

/* Data structures for the problem */
struct Edge{
    int a, b;
    Edge(){}
    Edge(int aa, int bb) : a(aa), b(bb){}
} edge[MAXE];

Set C;
Set bestC;
Set uncovered;


void init_(){

    for (int i = 1; i <= V; i++) {
        conf_change[i] = 1;
        dscore[i] = 0;
        timer_mod[i] = 0;
    }
    for (int i = 1; i <= E; i++) {
        edge_weight[i] = 1;
        dscore[edge[i].a] += edge_weight[i];
        dscore[edge[i].b] += edge_weight[i];
        // At first, all the edges are uncovered
        uncovered.insert(i);
    }



}

// Add vertex v into C and update other data accordingly.
void add(int v){
    C.insert(v);
    dscore[v] = -dscore[v];
    
}


void solve(){
    init_();
}



int main() {

    // POJ
    while (~scanf("%d%d", &V, &E)){
        // Build the graph
        int aa, bb;
        memset(G, 0, sizeof(G));
        memset(edge, 0, sizeof(edge));
        for (int i = 0; i < E; i++){
            scanf("%d%d", &aa, &bb);
            G[aa][bb] = 1;
            G[bb][aa] = 1;
        }
        int numedge = 1;
        for (int i = 1; i <= V; i++)
            for (int j = i + 1; j <= V; j++)
                if(G[i][j]){
                    edge[numedge].a = i;
                    edge[numedge++].b = j;
                }
        E = numedge - 1;


    }

    return 0;
}