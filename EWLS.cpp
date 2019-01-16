9/*
 NOTE:
 1. We use EWLS to solve Minimal Vertex Cover of complement graph(补图) \bar G.
    MaxClique(G) = V(G) - MinimalVertexCover(\bar G)
 
    **Thus G[][] actually is \bar G.**
 
 2.Weight of edges are stored in G[][]. Because G is an undirected graph thus
    the matrix G is always symmetric.
 
 3.Please delete '#define _DEBUG' before submitting the code.
 
 Current Score: 149
 
 */

#define _DEBUG          // delete this line before submitting!

#include <iostream>
#include <cstdio>
#include <cstring>
#include <set>
#include <list>
#include <vector>
#include <utility>

#define ioOptimizer ios_base::sync_with_stdio(0); cin.tie(0)
#define MAXN 760
#define INF (1 << 30)

using namespace std;

int N;
int tabuAdd, tabuRemove;
int G[MAXN][MAXN];
typedef set<int> Vertices;
struct Edge{
    int from, to;
    Edge(int f, int t){
        from = f;
        to = t;
    }
};

int cost(Vertices& C){
    int ans = 0;
    for (int i  = 0; i < N - 1; i++){
        if (C.find(i) != C.end())       // i in C
            continue;
        for (int j = i + 1; j < N; j++){
            if (C.find(j) != C.end())   // j in C
                continue;
            
            // both i and j are not in C
            ans += G[i][j];
        }
    }
    return ans;
}

int dscore(Vertices& C, int v){
    int ans = 0;
    for (int i = 0; i < N; i++){
        if (C.find(i) == C.end()){
            // i not in C, thus cost(C) will increase
            // if we remove v from C
            ans += G[v][i];
        }
    }
    if (C.find(v) != C.end()){
        // v in C, we have to reverse the sign of dscore
        ans = -ans;
    }
    return ans;
}

int score(Vertices& C, int u, int v){
#ifdef _DEBUG
    assert(C.find(u) != C.end());
    assert(C.find(v) == C.end());
#endif
    return dscore(C, u) + dscore(C, v) + G[u][v];
    // (u,v) is not an edge <=> G[u][v] = 0
}

void copy(Vertices& src, Vertices& des){
    // just shallow copy
    des.clear();
    for (Vertices::iterator i = src.begin(); i != src.end(); i++)
        des.insert(*i);
}

#ifdef _DEBUG
bool isSymmetric(int G[][MAXN], int n){
    // used for DEBUG
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            if (G[i][j] != G[j][i])
                return false;
        }
    }
    return true;
}
#endif

#ifdef _DEBUG
bool isClique(Vertices& C){
    // used for DEBUG
    for (Vertices::iterator i = C.begin(); i != C.end(); i++){
        for (Vertices:: iterator j = C.begin(); j != C.end(); j++){
            if (*i == *j)
                continue;
            if (G[*i][*j] == 1)
                return false;
        }
    }
    return true;
}
#endif

#ifdef _DEBUG
bool isVertexCover(Vertices& C){
    // used for DEBUG
    for (int i = 0; i < N; i++){
        for (int j = 0; j < N; j++){
            if (G[i][j] == 0)   // (i, j) is not an edge
                continue;
            
            bool find = false;  // i or j must be in C
            for (Vertices::iterator k = C.begin(); k != C.end(); k++){
                if (*k == i || *k == j){
                    find = true;
                    break; }
            }
            if (!find) return false;
        }
    }
    return true;
}
#endif

void remove(list<Edge>& L, list<Edge>::iterator unchecked, int v){
    // remove all edges in L with endpoint v
    for(list<Edge>::iterator i = L.begin(); i != L.end(); ){
        if ((*i).from == v || (*i).to == v){
            if (unchecked == i)
                unchecked++;
            L.erase(i++);
        }
        else
            i++;
    }
}

void insert(list<Edge>& L,  Vertices& C, int u){
    // insert edges {(u, i) | i not in C} to L
    for(int i = 0; i < N; i++){
        if (G[u][i] != 0 && C.find(i) == C.end()){
            // (u, i) exists and i is not in C
            // always insert to the front of L
            L.push_front(Edge(u, i));
        }
    }
}

void resize(Vertices& C, list<Edge>& L, int capacity){
    while (C.size() > capacity){
        // selects vertices with the highest dscore in C
        // TODO: breaking ties randomly
        int maxd = -INF;
        Vertices::iterator id;
        for (Vertices::iterator j = C.begin(); j != C.end(); j++){
            int d = dscore(C, *j);
            if(d > maxd){
                maxd = d;
                id = j;
            }
        }
        C.erase(id);
        
        // insert edges to L
        int u = *id;
        insert(L, C, u);
    }
}

void greedyVC(Vertices& C, list<Edge>& L, list<Edge>::iterator unchecked){
    // use greedy algo to expand C to a vertex cover
    while(!L.empty()){
        int maxcnt = 0;
        int id = -1;
        for (int i = 0; i < N; i++){
            if (C.find(i) != C.end())   // i in C <=> i not in L
                continue;
            int cnt = 0;                // count how many times i occurs in L
            for (list<Edge>::iterator j = L.begin(); j != L.end(); j++)
                if ((*j).from == i || (*j).to == i)
                    cnt++;

            if (cnt > maxcnt){
                maxcnt = cnt;
                id = i;
            }
        }
        remove(L, unchecked, id);
        C.insert(id);
    }
}

pair<int, int> ChooseSwapPair(Vertices& C, list<Edge>& L,
                              list<Edge>::iterator unchecked){
    // TODO: return random(s)
    int v;
    
    // check e(oldest)
    list<Edge>::iterator i = L.end();
    i--;
    v = (*i).from;
    for (int u = 0; u < N && v != tabuAdd; u++){
        if (C.find(u) != C.end() && u != tabuRemove && score(C, u, v) > 0)
            return make_pair(u, v);
    }
    v = (*i).to;
    for (int u = 0; u < N && v != tabuAdd; u++){
        if (C.find(u) != C.end() && u != tabuRemove && score(C, u, v) > 0)
            return make_pair(u, v);
    }
    
    // search UL from old to young
    do{
        unchecked--;
        v = (*unchecked).from;
        for (int u = 0; u < N && v != tabuAdd; u++){
            if (C.find(u) != C.end() && u != tabuRemove && score(C, u, v) > 0)
                return make_pair(u, v);
        }
        v = (*unchecked).to;
        for (int u = 0; u < N && v != tabuAdd; u++){
            if (C.find(u) != C.end() && u != tabuRemove && score(C, u, v) > 0)
                return make_pair(u, v);
        }
    }while(unchecked!=L.begin());
    
    // swappair not found
    return make_pair(0, 0);
}

void EWLS(int delta, int maxSteps, Vertices& Cstar){
    // init L and UL
    // UL ⊆ L is the set of those unchecked by ChooseSwapPair
    // in the current local search stage
    list<Edge> L;
    list<Edge>::iterator unchecked;
    for (int i = 0; i < N-1; i++)
        for (int j = i+1; j < N; j++)
            if (G[i][j] > 0)
                L.push_back(Edge(i, j));
    unchecked = L.end();
    
    // initialize C using greedy algo
    Vertices C;
    C.clear();
    while(!L.empty()){
        // vertex with the highest dscore is added to C
        // TODO: breaking ties randomly
        int maxd = -INF;
        int id = -1;
        for (int i = 0; i < N; i++){
            if (C.find(i) != C.end())   // i already in C
                continue;
            int d = dscore(C, i);
            if (d > maxd){
                maxd = d;
                id = i;
            }
        }
        remove(L, unchecked, id);
        C.insert(id);
    }
    
    copy(C, Cstar);                     // update Cstar
#ifdef _DEBUG
    assert(isVertexCover(Cstar));
#endif
    
    int ub = int(C.size());             // update
    resize(C, L, ub-delta);             // delete |delta| vertices from C
    
    
    for (int step = 0; step < maxSteps; step++){
#ifdef _DEBUG
        printf("round %d\n", step);
#endif
        pair<int, int> p = ChooseSwapPair(C, L, unchecked);
        int u = p.first;
        int v = p.second;
        
        if (u == 0 || v == 0){
            // this IF block corresponds to the ELSE block in Cai's Paper
            // update w(e) = w(e) + 1 for each e in L
           int randomL = (rand() % N); // pick random position of v
			        for (list<Edge>::iterator i = L.begin(); i != L.end(); i++) {
			            int from = (*i).from;
				           int to = (*i).to;

				           if (!randomL)
					          v = (*i).from; // get v
				           randomL--;

				           G[from][to]++;
				           G[to][from]++;
			         }
            
            // TODO: random chooose u
            u = (*C.begin());
            // v = (*L.begin()).from;
        }
        
        tabuAdd = u;
        tabuRemove = v;
        
        // update C by adding v and removing u
        C.erase(u);
        C.insert(v);
        
        // update L by adding edges(u, j) and removing edges(v, j)
        insert(L, C, u);
        remove(L, unchecked, v);
        
        if (C.size() + L.size() < ub){
            ub = int(C.size() + L.size());
            if (!L.empty()){
                greedyVC(C, L, unchecked);
            }
            
            copy(C, Cstar);
#ifdef _DEBUG
            printf("|Vertex Cover|: %d\n", int(Cstar.size()));
            assert(isVertexCover(Cstar));
#endif
            resize(C, L, ub-delta);     // delete |delta| vertices from C
        }
    }
#ifdef _DEBUG
    assert(isSymmetric(G, N));
#endif
}


void solve(int delta, int maxSteps){
    int M;
    Vertices C;
    bool ans[MAXN];
    while(scanf("%d%d", &N, &M) != EOF){
        C.clear();
        memset(ans, 0, sizeof(ans));
        for(int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                if (i == j)
                    continue;
                G[i][j] = 1;
            }
        }
        for(int i = 0; i < M; i++){
            int u, v;
            scanf("%d%d", &u, &v);
            // index starts from 0
            G[u-1][v-1] = 0;
            G[v-1][u-1] = 0;
        }
        
        EWLS(delta, maxSteps, C);
        
        printf("%d\n", int(N-C.size()));
        for (Vertices::iterator i = C.begin(); i != C.end(); i++){
            ans[*i] = 1;
        }
        Vertices A;
        for (int i = 0; i < N; i++){
            if (ans[i] == 0)
                A.insert(i);
        }
        
#ifdef _DEBUG
        assert(isClique(A));
#endif
        for (Vertices::iterator i = A.begin(); i != A.end(); i++){
            printf("%d ", (*i)+1);
        }
        printf("\n");
        
    }
}

void solveFile(const char* path, int delta, int maxSteps){
    int M;
    Vertices C;
    bool ans[MAXN];
    freopen(path, "r", stdin);
    if(scanf("%d %d ", &N, &M) != EOF){
        printf("N:%d M:%d\n", N, M);
        C.clear();
        memset(ans, 0, sizeof(ans));
        for(int i = 0; i < N; i++){
            for (int j = 0; j < N; j++){
                if (i == j)
                    continue;
                G[i][j] = 1;
            }
        }
        for(int i = 0; i < M; i++){
            int u, v;
            scanf("%d %d ", &u, &v);
            // if (i < 5 || i > M - 5)
                // printf("%d %d\n", u, v);
            // index starts from 0
            G[u-1][v-1] = 0;
            G[v-1][u-1] = 0;
        }
        
        EWLS(delta, maxSteps, C);
        
        printf("%d\n", int(N-C.size()));
        for (Vertices::iterator i = C.begin(); i != C.end(); i++){
            ans[*i] = 1;
        }
        Vertices A;
        for (int i = 0; i < N; i++){
            if (ans[i] == 0)
                A.insert(i);
        }
        
#ifdef DEBUG
        assert(isClique(A));
#endif
        for (Vertices::iterator i = A.begin(); i != A.end(); i++){
            printf("%d ", (*i)+1);
        }
        printf("\n");
    }
}

int main(){
    // try different params!

    // Openjudge
    // solve(10, 200);
    
    // Local test sample
    // const char* path = "/samples/frb30.txt";
    // solveFile(path, 10, 200);
    return 0;
}
