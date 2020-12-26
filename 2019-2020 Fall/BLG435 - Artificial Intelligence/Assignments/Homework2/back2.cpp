#include <cstdio>
#include <iostream>
#include <unordered_map>

using namespace std;

unordered_map<long long, pair<int, int>> DP;

int created_nodes;


struct Node {
    // in order to reduce the memory taken and easily memoize the solution (aka dynamic programming), 
    // we can hold the state in a number (in this case long long) using bitmask (also this value will be our hash value).
    // we need:
    // - 1 bit for player turn (minimize or maximize) (M)
    // - 12 bits for vertical edges (V)
    // - 12 bits for horizontal edges (H)
    // - 4 bit for score of player 1 (A)
    // - 4 bit for score of player 2 (B)
    // Hence we can use a long long integer to represent the state (because we need 33 bits at least) -> {? x 31}{M x 1}{V x 12}{H x 12}{A x 4}{B x 4}
    // You can also see the report for detailed explanation.

    int R, C;
    long long state;

    Node(int R, int C) {
        this->R = R;
        this->C = C;
    }

    int empty_edge_count() {
        int result = 0;

        for (int index = 0; index < (R + 1) * C; index++)
            result += !get_horizontal_edge(index);

        for (int index = 0; index < R * (C + 1); index++)
            result += !get_vertical_edge(index);
        
        return result;
    }

    void set_maximize(bool value) {
        if (get_maximize() != value)
            state ^= (1 << (R * (C + 1) + C * (R + 1) + 10));
    }

    bool get_maximize() {
        return state & (1 << (R * (C + 1) + C * (R + 1) + 10));
    }

    void set_scores(int s1, int s2) {
        int mask = (s1 << 4) | s2;

        state >>= 8;    // clear scores
        state <<= 8;
        state ^= mask;  // set scores
    }

    pair<int, int> get_scores() {
        int mask = (1LL << 4) - 1;
        int s2 = state & mask;
        
        mask <<= 4;
        int s1 = (state & mask) >> 4;
        
        return {s1, s2};
    }

    void set_vertical_edge(int index, bool value) {
        if (get_vertical_edge(index) != value)
            state ^= (1 << (((R + 1) * C) + index + 10));
    }

    bool get_vertical_edge(int index) {
        return state & (1 << (((R + 1) * C) + index + 10));
    }

    void set_horizontal_edge(int index, bool value) {
        if (get_horizontal_edge(index) != value) 
            state ^= (1 << (index + 10));
    }

    bool get_horizontal_edge(int index) {
        return state & (1 << (index + 10));
    }

    int gain_of_horizontal_edge(int i, int j) {
        int gain = 0;
        int index = (i * C) + j;
        
        if (i > 0) {
            bool U = get_horizontal_edge(index - C);
            bool L = get_vertical_edge((i - 1) * (C + 1) + j);
            bool R = get_vertical_edge((i - 1) * (C + 1) + j + 1);

            gain += (U and L and R);
        }

        if (i < R) {
            bool D = get_horizontal_edge(index + C);
            bool L = get_vertical_edge(i * (C + 1) + j);
            bool R = get_vertical_edge(i * (C + 1) + j + 1);

            gain += (D and L and R);
        }

        return gain;
    }

    int gain_of_vertical_edge(int i, int j) {
        int gain = 0;
        int index = (i * (C + 1)) + j;

        if (j > 0) {
            bool L = get_vertical_edge(index - 1);
            bool U = get_horizontal_edge(i * C + j - 1);
            bool D = get_horizontal_edge((i + 1) * C + j - 1);

            gain += (L and U and D);
        }

        if (j < C) {
            bool R = get_vertical_edge(index + 1);
            bool U = get_horizontal_edge(i * C + j);
            bool D = get_horizontal_edge((i + 1) * C + j);

            gain += (R & U & D);
        }

        return gain;
    }
};


pair<int, int> minimax(Node* node) {
    if (!node->empty_edge_count()) 
        return node->get_scores();

    if (DP.find(node->state) != DP.end())  // if we encounter this state before, we can directly return the saved answer
        return DP[node->state];
    
    pair<int, int> ans = {-10, 10};

    if (!node->get_maximize())
        ans.first = 20;

    for (int i=0; i < node->R + 1; i++) {  // select an empty horizontal edge
        for (int j=0; j < node->C; j++) {
            int index = (i * node->C) + j;
            
            if (node->get_horizontal_edge(index))
                continue;
    
            int gain = node->gain_of_horizontal_edge(i, j);
            created_nodes++;
            
            Node* next = new Node(*node);
            pair<int, int> score = next->get_scores();
            
            if (node->get_maximize())
                next->set_scores(score.first + gain, score.second);
            else
                next->set_scores(score.first, score.second + gain);
            next->set_horizontal_edge(index, true);

            if (gain == 0)
                next->set_maximize(!node->get_maximize());

            pair<int, int> sub = minimax(next);
            delete next;

            if (node->get_maximize() and sub.first - sub.second > ans.first - ans.second)
                ans = sub;
            else if (!node->get_maximize() and sub.first - sub.second < ans.first - ans.second)
                ans = sub;
        }
    }

    for (int i=0; i < node->R; i++) {  // select an empty vertical edge
        for (int j=0; j < node->C + 1; j++) {
            int index = (i * (node->C + 1)) + j;
            
            if (node->get_vertical_edge(index))
                continue;
    
            int gain = node->gain_of_vertical_edge(i, j);
            created_nodes++;

            Node* next = new Node(*node);
            pair<int, int> score = next->get_scores();

            if (node->get_maximize())
                next->set_scores(score.first + gain, score.second);
            else
                next->set_scores(score.first, score.second + gain);

            next->set_vertical_edge(index, true);

            if (gain == 0)
                next->set_maximize(!node->get_maximize());        

            pair<int, int> sub = minimax(next);
            delete next;

            if (node->get_maximize() and sub.first - sub.second > ans.first - ans.second)
                ans = sub;
            else if (!node->get_maximize() and sub.first - sub.second < ans.first - ans.second)
                ans = sub;
        }
    }

    return DP[node->state] = ans;  // save the state and return the answer
}

pair<int, int> minimax(Node* node, int alpha, int beta) {
    if (!node->empty_edge_count()) 
        return node->get_scores();

    if (DP.find(node->state) != DP.end())  // if we encounter this state before, we can directly return the saved answer
        return DP[node->state];
    
    pair<int, int> ans = {-10, 10};

    if (!node->get_maximize())
        ans.first = 20;

    int utility = ans.first - ans.second;
    int flag = true;

    for (int i=0; i < node->R + 1 && flag; i++) {  // select an empty horizontal edge
        for (int j=0; j < node->C && flag; j++) {
            int index = (i * node->C) + j;
            
            if (node->get_horizontal_edge(index))
                continue;
    
            int gain = node->gain_of_horizontal_edge(i, j);
            created_nodes++;
            
            Node* next = new Node(*node);
            pair<int, int> score = next->get_scores();
            
            if (node->get_maximize())
                next->set_scores(score.first + gain, score.second);
            else
                next->set_scores(score.first, score.second + gain);
            next->set_horizontal_edge(index, true);

            if (gain == 0)
                next->set_maximize(!node->get_maximize());

            pair<int, int> sub = minimax(next, alpha, beta);
            delete next;

            int s_utility = sub.first - sub.second;

            if (node->get_maximize()) {
                if (utility < s_utility) {
                    utility = s_utility;
                    ans = sub;
                }
                if (gain != 0)
                    alpha = max(alpha, utility);
            }

            else if (!node->get_maximize()){
                if (utility > s_utility) {
                    utility = s_utility;
                    ans = sub;
                }
                if (gain != 0)
                    beta = min(beta, utility);
            }
            if (alpha >= beta)
                flag = false;    
        }
    }

    for (int i=0; i < node->R && flag; i++) {  // select an empty vertical edge
        for (int j=0; j < node->C + 1 && flag; j++) {
            int index = (i * (node->C + 1)) + j;
            
            if (node->get_vertical_edge(index))
                continue;
    
            int gain = node->gain_of_vertical_edge(i, j);
            created_nodes++;

            Node* next = new Node(*node);
            pair<int, int> score = next->get_scores();

            if (node->get_maximize())
                next->set_scores(score.first + gain, score.second);
            else
                next->set_scores(score.first, score.second + gain);

            next->set_vertical_edge(index, true);

            if (gain == 0)
                next->set_maximize(!node->get_maximize());        

            pair<int, int> sub = minimax(next, alpha, beta);
            delete next;

            int s_utility = sub.first - sub.second;

            if (node->get_maximize()) {
                if (utility < s_utility) {
                    utility = s_utility;
                    ans = sub;
                }
                if (gain != 0)
                    alpha = max(alpha, utility);
            }

            else if (!node->get_maximize()){
                if (utility > s_utility) {
                    utility = s_utility;
                    ans = sub;
                }
                if (gain != 0)
                    beta = min(beta, utility);
            }
            if (alpha >= beta)
                flag = false;  
        }
    }

    return DP[node->state] = ans;  // save the state and return the answer
}



int main(int argc, const char* argv[]) {
    if (argc < 2 and false) {
        cerr << "Usage: " << argv[0] << " <input-file>" << endl;
        return 0;
    }

    // freopen(argv[1], "r", stdin);

    int R, C, x, score1, score2;
    scanf("%d%d", &R, &C);

    Node node(R, C);  // initial node
    created_nodes++;

    for (int i=0; i < C * (R + 1); i++) {
        scanf("%d", &x);
        node.set_horizontal_edge(i, x);
    }

    for (int i=0; i < (C + 1) * R; i++) {
        scanf("%d", &x);
        node.set_vertical_edge(i, x);
    }

    scanf("%d%d", &score1, &score2);
    node.set_scores(score1, score2);
    node.set_maximize(true);

    pair<int, int> ans = minimax(&node);
    cout << ans.first << endl;
    cout << ans.second << endl;
    cout << "Created Nodes: " << created_nodes << endl;
    
    return 0;
}