/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Compilation: g++ kod.cpp -o kod -std=c++11
 * Running Command: ./kod dfs
 * Note: For understanding the state logic, please check report
 */

#include <queue>
#include <chrono>
#include <vector>
#include <cstring>  // memset function
#include <iostream>  // standart I/O functions
#include <algorithm>  // max function

using namespace std;


class Game {
    int node_cnt;  //  number of visited nodes
    int max_node_cnt;  // maximum number of nodes kept in memory
    
    int trace[16];  // it holds the state transitions
    bool visited[16];  // it holds the state is visited or not
    bool solved;  // it holds the problem is solved or not
    string names[4] = {"Carrot", "Rabbit", "Fox", "Farmer"};
    
public:
    Game(){
        solved = false;
        node_cnt = 0;
        max_node_cnt = 0;
        memset(visited, false, sizeof visited);  // make sure that all stages are unvisited
    }
    
    void run(string algorithm);
    bool bfs();
    bool dfs(int state=0, int previous=0, int depth=1);

    bool check_state(int state);
    void print_state(int state);
    void print_move(int previos, int next);
};


bool Game::dfs(int state, int previous, int depth){
    node_cnt++;
    max_node_cnt = max(max_node_cnt, depth);
    
    trace[state] = previous;  // it means to reach the state, first you have to reach "previous" state
    
    if (state == 15) // final condition
        return solved = true;
    
    visited[state] = true;  // mark the current state visited
    
    // farmer travels alone if possible
    if (!visited[state ^ 8] && check_state(state ^ 8) && dfs(state ^ 8, state, depth+1))
        return true;

    // farmer travels with one other if they are at the same side
    for (int i=1; i < 8; i <<= 1)
        if (((state & 8) | (state & i)) > 8 || ((state & 8) | (state & i)) == 0)
            if (!visited[state ^ (8 ^ i)] && check_state(state ^ (8 ^ i)) && dfs(state ^ (8 ^ i), state, depth+1))
                return true;
    
    return solved = false;
}


bool Game::bfs(){
    queue<pair<int, int>> Q;  // holds <current_state, previous_state> pairs
    Q.push(make_pair(0, 0));  // start with <0, 0>
    
    pair<int, int> current;
    int state, previous;
    
    while (!Q.empty()){
        node_cnt++;
        max_node_cnt = max(max_node_cnt, (int)Q.size());
        
        current = Q.front(); Q.pop();  // fetch and assign next state
        state = current.first;
        previous = current.second;
        
        visited[state] = true;  // mark the current state visited
        trace[state] = previous;  // it means to reach the state, first you have to reach "previous" state
        
        if (state == 15)  // final condition
            return solved = true;
        
        // farmer travels alone if possible
        if (!visited[state ^ 8] && check_state(state ^ 8))
            Q.push(make_pair(state ^ 8, state));
        
        // farmer travels with one other if they are at the same side
        for (int i=1; i < 8; i <<= 1)
            if (((state & 8) | (state & i)) > 8 || ((state & 8) | (state & i)) == 0)
                if (!visited[state ^ (8 ^ i)] && check_state(state ^ (8 ^ i)))
                    Q.push(make_pair(state ^ (8 ^ i), state));
    }
    
    return solved = false;
}


void Game::run(string algorithm){
    auto begin = std::chrono::steady_clock::now();
    
    if (algorithm == "DFS" || algorithm == "dfs")
        dfs();
    
    else if (algorithm == "BFS" || algorithm == "bfs")
        bfs();
    
    else{
        printf("Invalid Algorithm! Try DFS or BFS\n");
        return;
    }
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::steady_clock::now() - begin);
    
    printf("Algorithm: %s\n", algorithm.c_str());
    printf("Number of the visited nodes: %d\n", node_cnt);
    printf("Maximum number of nodes kept in the memory: %d\n", max_node_cnt);
    printf("Running time: %lld milliseconds\n", duration.count());
    
    int state = 15;
    vector<int> path;
    
    // find the path following the trace
    while (solved){
        path.push_back(state);
        state = trace[state];
        if (state == 0){
            path.push_back(state);
            break;
        }
    }
    
    // print the state and moves
    printf("Solution move count: %lu\n", path.size()-1);
    for (int i=(int)path.size()-1; 0 <= i; i--){
        print_state(path[i]);
        if (i != 0)
            print_move(path[i], path[i-1]);
    }
}


void Game::print_state(int state){
    for (int j=8, index=3; 1 <= j; j >>= 1, index--)
        if ((state & j) == 0)
            printf("%s ", names[index].c_str());
    
    printf("|| ");
    
    for (int j=8, index=3; 1 <= j; j >>= 1, index--)
        if (state & j)
            printf("%s ", names[index].c_str());
    printf("\n");
}


void Game::print_move(int previous, int next){
    printf("( ");
    for (int j=8, index=3; j >= 1; j >>= 1, index--)
        if ((previous & j) != (next & j))
            printf("%s, ", names[index].c_str());
    printf("%s )\n", previous & 8 ? "<" : ">");
}


bool Game::check_state(int state){
    // check report for understanding the states meaning
    // (state & 14) == 6 covers the state 6 (0110) and state 7 (0111)
    // (state & 11) == 3 covers the state 3 (0011) and state 7 (0111)
    // (state & 14) == 8 covers the state 8 (1000) and state 9 (1001)
    // (state & 11) == 8 covers the state 8 (1000) and state 12 (1100)
    return !((state & 14) == 6 || (state & 11) == 3 || (state & 14) == 8 || (state & 11) == 8);
}


int main(int argc, const char * argv[]) {
    if (argc < 2){
        printf("Algorithm missing! Please specify the algorithm such that: ./app bfs\n");
        return 0;
    }
    
    Game game;
    game.run(argv[1]);
    
    return 0;
}
