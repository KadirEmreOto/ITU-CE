//
//  Grammer.hpp
//  Project
//
//  Created by Kadir Emre Oto on 21.11.2018.
//  Copyright © 2018 Kadir Emre Oto. All rights reserved.
//

#ifndef Grammer_hpp
#define Grammer_hpp

#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_set>
#include <unordered_map>

#include "Symbol.hpp"

class Grammar {
    Symbol* start;
    std::unordered_map<std::string, Symbol*> symbols;
    
public:
    Grammar();
    ~Grammar();
    
    Symbol* get_symbol(const char* name);
    void add_transition(Symbol* L, std::vector<Symbol*> R);
    
    void calculate_first_sets();
    void calculate_follow_sets();
    
    void load(const char* filename);
    void display();
};


#endif /* Grammer_hpp */
