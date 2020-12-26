//
//  Symbol.cpp
//  Project
//
//  Created by Kadir Emre Oto on 7.12.2018.
//  Copyright © 2018 Kadir Emre Oto. All rights reserved.
//

#include "Symbol.hpp"

Symbol::Symbol(const char* name): name(name){
    is_visited = is_first_calculated = is_follow_calculated = false;
}

void Symbol::add_transition(std::vector<Symbol*> nodes){
    transitions.insert(nodes);
}

void Symbol::calculate_first(){
    // This function is deprecated, grammar class can calculate first sets
    
    if (is_first_calculated)
        return;
    
    is_visited = true;
    
    if (type != Type::nonterminal){
        first.insert(this);
        is_first_calculated = true;
        return;
    }
    
    bool stack_flag = false;
    for (auto& vc : transitions){
        Symbol* empty = nullptr;
        
        for (auto& it : vc){
            if (it->is_visited and it->type == Symbol::nonterminal){
                it->S.push(this);
                stack_flag = true;
                break;
            }
            
            it->calculate_first();
            bool can_be_empty = false;
            
            for (auto& tmp : it->first){
                if (tmp->type == Symbol::empty){
                    can_be_empty = true;
                    empty = tmp;
                }
                
                else
                    first.insert(tmp);
            }
            
            if (!can_be_empty){
                empty = nullptr;
                break;
            }
        }
        
        if (empty != nullptr)
            first.insert(empty);
    }
    
    
    if (!stack_flag){
        is_first_calculated = true;
        //is_visited = false;  // TODO: maybe it is not necessary
    }
    

    while (S.size() > 0){
        S.top()->calculate_first();
        S.pop();
    }
    
}

void Symbol::calculate_follow(){
    // This function is deprecated, grammar class can calculate follow sets
    
    for (auto& vc : transitions){
        for (int i=0; i < vc.size() - 1; i++){
            for (int j=i+1; j < vc.size(); j++){
                bool can_be_empty = false;
                vc[j]->calculate_first();  // be sure that first set is calculated
                
                for (auto& sym : vc[j]->first){
                    if (sym->type == Symbol::empty)
                        can_be_empty = true;
                    else
                        vc[i]->follow.insert(sym);
                }
                
                if (!can_be_empty)
                    break;
            }
        }
        
        
    }
}
