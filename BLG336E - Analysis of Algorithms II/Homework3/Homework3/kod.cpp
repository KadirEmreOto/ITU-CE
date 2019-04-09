/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Compilation: g++ kod.cpp -o kod -O2 -std=c++11
 * Running Command: ./kod data.txt or ./kod < data.txt
 */

#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Graph {
    vector<int> *relation;  // holds neigborhood relations (for faster access)
    int **weight;  // holds the weights of edges, it will change after the execution of flow
    int **refill;  // holds the initial weights of edges,
    int *parent;   // holds the parent relations for finding paths in BFS
    const int size;  // number of nodes in graph
    
public:
    Graph(int size);
    ~Graph();
    
    void add_edge(int u, int v, int w=1);
    void change_weight(int u, int v, int w);

    int farthest(int node);
    bool BFS(int source, int sink);
    int flow(int source, int sink);

    void rewind();
};

Graph::Graph(int size): size(size){  // constructor
    // dynamic allocations
    
    relation = new vector<int> [size + 1];
    weight = new int* [size + 1];
    refill = new int* [size + 1];
    parent = new int [size+1]();
    
    for (int i=0; i < size + 1; i++){
        weight[i] = new int [size + 1];
        refill[i] = new int [size + 1];
    }
}

Graph::~Graph(){  // destructor
    // free all allocated memories
    
    for (int i=0; i < size + 1; i++){
        delete [] weight[i];
        delete [] refill[i];
    }
    
    delete [] relation;
    delete [] weight;
    delete [] refill;
    delete [] parent;
}

void Graph::add_edge(int u, int v, int w){  // adds an edge to graph
    relation[u].push_back(v);
    relation[v].push_back(u);
    
    weight[u][v] = refill[u][v] = w;
}

void Graph::change_weight(int u, int v, int w){  // changes weight of given edge
    weight[u][v] = w;
}

bool Graph::BFS(int source, int sink){  // finds a path from source to sink (Breadth First Search)
    queue<int> q;
    q.push(source);
    
    bool *visited = new bool[size + 1]();  // The values are initially false
    visited[source] = true;
    
    int u, v;
    
    // there is a path from u to v, (u -> v), u = parent[v]
    while (!q.empty()) {
        u = q.front();
        q.pop();
        
        for (int i=0; i < relation[u].size(); i++){
            v = relation[u][i];
            
            if (!visited[v] && weight[u][v] > 0){
                q.push(v);
                parent[v] = u;  // set the parent[v]
                visited[v] = true;
                
                if (v == sink)
                    return true;
            }
        }
    }
    
    return false;
}

int Graph::farthest(int node){  // finds farthest node to given node (assumes all edges unit) (uses BFS)
    queue<int> q;
    q.push(node);
    
    bool *visited = new bool[size + 1]();  // The values are initially false
    visited[node] = true;

    while (!q.empty()) {
        node = q.front();
        q.pop();
        
        for (int i=0, it; i < relation[node].size(); i++){
            it = relation[node][i];
            
            if (!visited[it] && weight[node][it] > 0){  // if the node is not visited before and the edge is open
                q.push(it);
                visited[it] = true;
            }
        }
    }
    
    // the last node we visit is the farthest node to given node
    return node;
}

int Graph::flow(int source, int sink){  // finds the max-flow (min-cut) from source to sink
    int answer = 0, u, v;
    
    // there is a path from u to v, (u -> v), u = parent[v]
    while (BFS(source, sink)){  // while a path exist from source to sink
        v = sink;
        answer++;
        
        while (v != source){
            u = parent[v];
        
            weight[u][v]--;  // change capacity
            weight[v][u]++;  // change residual capacity
        
            v = parent[v];
        }
    }
    
    return answer;
}

void Graph::rewind(){
    // rewinds the initial graph before flow
    for (int i=0; i < size + 1; i++)
        for (int j=0; j < size + 1; j++)
            weight[i][j] = refill[i][j];
}


int solve(const char* filename=NULL){
    if (filename)
        freopen(filename, "r", stdin);  // additionally faster that ifstream
    
    int N, M, u, v;
    scanf("%d%d", &N, &M);  // read the N and M, representing the number of nodes and edges respectively
    
    Graph G(N*2 + 1);
    
    for (int i = 0; i < M; i++){  // build graph
        scanf("%d%d", &u, &v);    // read and add edges
        G.add_edge(u, u + N);     // u -> u' (u' = u + N) (u' is the copy of u)
        G.add_edge(v, v + N);     // v -> v' (v' = v + N) (v' is the copy of v)
        
        G.add_edge(u + N, v);     // u' -> v
        G.add_edge(v + N, u);     // v' -> u
    }
    
    int answer = 1e6;
    
    for (int source=1, sink; source < N; source++){  // for all nodes
        G.rewind();  // rewind the graph, because flow algorithm ruins initial graph
        sink = G.farthest(source) - N;  // find the farthest node to source
        
        G.change_weight(source, source + N, 1e6);  // we don't want to discard the source while finding the flow
        answer = min(answer, G.flow(source, sink));  // update the answer
    }
    
    return answer;
}


int main(int argc, const char * argv[]) {
    if (argc > 1)  // reads from given file
        cout << solve(argv[1]) << endl;
    else  // reads from stdin
        cout << solve() << endl;
        
    return 0;
}
