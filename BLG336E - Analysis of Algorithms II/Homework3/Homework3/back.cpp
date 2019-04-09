#include <iostream>
#include <vector>
#include <queue>

using namespace std;

class Graph {
    vector<int> *G;
    int **W;
    int **R;
    int *P;
    const int size;
    
public:
    Graph(int size);
    ~Graph();
    
    void add_edge(int u, int v, int w=1);
    
    void rewind();
    int farthest(int node);
    int flow(int source, int sink);
    bool BFS(int source, int sink);
};

Graph::Graph(int size): size(size){
    G = new vector<int> [size + 1];
    W = new int* [size + 1];
    R = new int* [size + 1];
    P = new int [size+1]();
    
    for (int i=0; i < size + 1; i++){
        W[i] = new int [size + 1];
        R[i] = new int [size + 1];
    }
}

Graph::~Graph(){
    for (int i=0; i < size + 1; i++){
        delete [] W[i];
        delete [] R[i];
    }
    
    delete [] G;
    delete [] W;
    delete [] R;
    delete [] P;
}

void Graph::add_edge(int u, int v, int w){
    G[u].push_back(v);
    G[v].push_back(u);
    
    W[u][v] = R[u][v] = w;
}

bool Graph::BFS(int source, int sink){
    bool *visited = new bool[size + 1]();  // The values are initially false
    
    queue<int> q;
    q.push(source);
    visited[source] = true;
    
    int u, v;
    
    while (!q.empty()) {
        u = q.front();
        q.pop();
        
        for (int i=0; i < G[u].size(); i++){
            v = G[u][i];
            
            if (!visited[v] && W[u][v] > 0){
                q.push(v);
                P[v] = u;
                visited[v] = true;
                
                if (v == sink){
                    return true;
                }
            }
        }
    }
    
    return false;
}

int Graph::farthest(int node){
    bool *visited = new bool[size + 1]();  // The values are initially false
    
    queue<int> q;
    q.push(node);
    visited[node] = true;

    while (!q.empty()) {
        node = q.front();
        q.pop();
        
        for (int i=0, it; i < G[node].size(); i++){
            it = G[node][i];
            
            if (!visited[it]){
                q.push(it);
                
                visited[it] = true;
            }
        }
    }
    
    return node;
}

int Graph::flow(int source, int sink){
    int answer = 0, u, v;
    
    while (BFS(source, sink)){
        v = sink;
        answer++;
        
        while (v != source){
            u = P[v];
        
            W[u][v]--;
            W[v][u]++;
        
            v = P[v];
        }
    }
    
    return answer;
}

void Graph::rewind(){
    for (int i=0; i < size + 1; i++)
        for (int j=0; j < size + 1; j++)
            W[i][j] = R[i][j];
}


int main(int argc, const char * argv[]) {
    Graph G(14);
    
    G.add_edge(1, 7, 1e5);
    G.add_edge(7, 2);
    G.add_edge(7, 3);
    
    G.add_edge(2, 8);
    G.add_edge(8, 3);
    G.add_edge(8, 4);
    
    G.add_edge(3, 9);
    G.add_edge(9, 2);
    G.add_edge(9, 4);
    G.add_edge(9, 5);
    
    G.add_edge(4, 10);
    G.add_edge(10, 2);
    G.add_edge(10, 3);
    G.add_edge(10, 6);
    
    G.add_edge(5, 11);
    G.add_edge(11, 3);
    G.add_edge(11, 6);
    
    G.add_edge(6, 12, 1e5);
    G.add_edge(12, 4);
    G.add_edge(12, 5);
    
    cout << G.flow(1, 12) << endl;
    return 0;
}
