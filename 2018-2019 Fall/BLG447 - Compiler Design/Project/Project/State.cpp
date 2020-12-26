//
//  State.cpp
//  Project
//
//  Created by Kadir Emre Oto on 25.12.2018.
//  Copyright © 2018 Kadir Emre Oto. All rights reserved.
//

#include "State.hpp"

State::State(std::vector<Symbol*> T){
    transitions.insert(T);
    closure();
}

void State::closure(){
    size_t previous_size = 0;  // to determine if the closure set is changed
    
    // std::set<std::vector<Symbol*>> transitions;
    while (previous_size != transitions.size()){
        previous_size = transitions.size();
        
        for (auto& vec : transitions){
            for (auto& sym : vec){
                
            }
            
        }
    }
}
