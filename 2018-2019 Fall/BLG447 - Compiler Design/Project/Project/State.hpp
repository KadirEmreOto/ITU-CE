//
//  State.hpp
//  Project
//
//  Created by Kadir Emre Oto on 25.12.2018.
//  Copyright © 2018 Kadir Emre Oto. All rights reserved.
//

#ifndef State_hpp
#define State_hpp

#include "Symbol.hpp"
#include <map>
#include <set>
#include <vector>


struct State {
    int id;
    std::map<Symbol*, State*> children;
    std::set<std::vector<Symbol*>> transitions;
    
    State(std::vector<Symbol*>);
    void closure();
};

#endif /* State_hpp */
