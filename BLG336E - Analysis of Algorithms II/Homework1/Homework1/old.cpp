#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int vnodes, maxvnodes;

vector <int> trace;
bool visited[16];
string names[4] = {"Carrot", "Rabbit", "Fox", "Farmer"};


bool dfs(int state){
    vnodes++;
    if (state == 15)
        return true;
    
    if (visited[state])
        return false;
    
    visited[state] = true;
    
    if ((state & 14) == 6 || (state & 11) == 3 || (state & 14) == 8 || (state & 11) == 8)
        return false;
    
    trace.push_back(state);
    maxvnodes = max(maxvnodes, (int)trace.size());
    
    if (dfs(state ^ 8))
        return true;

    for (int i=1; i < 8; i <<= 1)
        if (((state & 8) | (state & i)) > 8 || ((state & 8) | (state & i)) == 0)
            if (dfs(state ^ (8 ^ i)))
                return true;
                
    trace.pop_back();
    return false;
}


void display(){
    trace.push_back(15);
    
    printf("Algorithm: DFS\n");
    printf("Number of the visited nodes: %d\n", vnodes);
    printf("Maximum number of nodes kept in the memory: %d\n", maxvnodes);
    
    printf("Solution move count: %lu\n", trace.size() - 1);
    for (int i=0; i < trace.size(); i++){
        for (int j=1, index=0; j <= 8; j <<= 1, index++)
            if ((trace[i] & j) == 0)
                printf("%s ", names[index].c_str());
            
        printf("|| ");
            
        for (int j=1, index=0; j <= 8; j <<= 1, index++)
            if (trace[i] & j)
                printf("%s ", names[index].c_str());
        printf("\n");
        
        if (i != trace.size()-1){
            printf("(");
            for (int j=8, index=3; j >= 1; j >>= 1, index--)
                if ((trace[i] & j) != (trace[i+1] & j))
                    printf("%s, ", names[index].c_str());
            printf("%c)\n", trace[i] & 8 ? '<': '>');
        }
    }
}


int main(int argc, const char * argv[]) {
    dfs(0);
    display();
    return 0;
}
