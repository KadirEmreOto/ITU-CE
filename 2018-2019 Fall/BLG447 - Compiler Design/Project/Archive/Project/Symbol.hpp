//
//  Symbol.hpp
//  Project
//
//  Created by Kadir Emre Oto on 7.12.2018.
//  Copyright © 2018 Kadir Emre Oto. All rights reserved.
//

#ifndef Symbol_hpp
#define Symbol_hpp

#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <unordered_set>


class Symbol{
    friend class Grammar;
    std::stack<Symbol*> S;
    
    bool is_visited;
    bool is_first_calculated;
    bool is_follow_calculated;
    
public:
    enum Type {
        nonterminal = 1,
        terminal = 2,
        empty = 3,
        end = 4
    };
    
    int type;
    const std::string name;
    
    std::unordered_set<Symbol*> first, follow;
    std::set<std::vector<Symbol*>> transitions;
    
    Symbol(const char* name);
    Type get_type(const char* name);
    
    void calculate_first();  // deprecated
    void calculate_follow();  // deprecated

    void add_transition(std::vector<Symbol*>);
};

#endif /* Symbol_hpp */
