//
//  Grammer.cpp
//  Project
//
//  Created by Kadir Emre Oto on 21.11.2018.
//  Copyright © 2018 Kadir Emre Oto. All rights reserved.
//

#include "Grammar.hpp"

Grammar::Grammar(){
    start = nullptr;
}

Grammar::~Grammar(){
    for (auto& it : symbols)
        if (it.second)
            delete it.second;
}

Symbol* Grammar::get_symbol(const char* name){
    if (symbols.find(name) == symbols.end()){
        symbols[name] = new Symbol(name);
        
        if (symbols[name]->name == "$")
            symbols[name]->type = Symbol::end;
        else if (symbols[name]->name == "%")
            symbols[name]->type = Symbol::empty;
    }
    
    return symbols[name];
}

void Grammar::add_transition(Symbol *L, std::vector<Symbol*> R){
    L->add_transition(R);
}

void Grammar::load(const char* filename){
    std::ifstream input;
    input.open(filename);
    
    if (!input)
        exit(1);
    
    std::string left, right, symbol;
    std::unordered_set<Symbol*> nonterminals;
    
    while (getline(input, left, ' ')) {
        getline(input, right, '>');
        getline(input, right, '\n');
        
        std::stringstream ss(right);
        
        Symbol* L = get_symbol(left.c_str());
        nonterminals.insert(L);
        
        if (start == nullptr)
            start = L;
        
        std::vector<Symbol*> nodes;
        while (ss >> symbol)
            nodes.push_back(get_symbol(symbol.c_str()));
        
        add_transition(L, nodes);
    }
    
    for (auto& sym : symbols){
        if (nonterminals.find(sym.second) != nonterminals.end())
            sym.second->type = Symbol::nonterminal;
        
        else if (sym.second->name == "%")
            sym.second->type = Symbol::empty;
        
        else if (sym.second->name == "$")
            sym.second->type = Symbol::end;
        else
            sym.second->type = Symbol::terminal;
    }
        
}

void Grammar::calculate_first_sets(){
    get_symbol("$")->first.insert(get_symbol("$"));
    
    for (auto& it : symbols){  // Rule 1, 2, and 3 in course slide
        if (it.second->type == Symbol::terminal)
            it.second->first.insert(it.second);
        
        else if (it.second->type == Symbol::empty)
            it.second->first.insert(it.second);
        
        else if (it.second->type == Symbol::nonterminal){
            for (auto& sym : it.second->transitions){
                if (sym.front()->type == Symbol::terminal)
                    it.second->first.insert(sym.front());
                if (sym.front()->type == Symbol::empty)
                    it.second->first.insert(sym.front());
            }
        }
    }
    
    bool is_changed = true;
    
    while (is_changed){
        is_changed = false;
        
        for (auto& it : symbols){  // Rule 4 and 5 in course slide
            Symbol* X = it.second;
            
            if (X->type == Symbol::nonterminal){
                for (auto& vec : X->transitions){
                    bool can_be_empty = true;
                    Symbol* empty = nullptr;
                    
                    for (auto& Y : vec){
                        bool flag = false;
                        
                        for (auto& f : Y->first){
                            if (f->type == Symbol::empty){
                                flag = true;
                                empty = f;
                            }
                            
                            else
                                if (X->first.find(f) == X->first.end()){
                                    X->first.insert(f);
                                    is_changed = true;
                                }
                        }
                        if (!flag){
                            can_be_empty = false;
                            break;
                        }
                    }
                    
                    if (can_be_empty and X->first.find(empty) == X->first.end()){
                        X->first.insert(empty);
                        is_changed = true;
                    }
                }
            }
        }
    }
    
    
}

void Grammar::calculate_follow_sets(){
    start->follow.insert(get_symbol("$"));  // Rule 1 in course slide
    
    bool is_changed = true;
    
    while (is_changed){
        is_changed = false;
        
        for (auto& it : symbols){  // Rule 2 and 3 in course slide
            Symbol* A = it.second;
            
            if (A->type == Symbol::nonterminal){
                for (auto& vec : A->transitions){
                    for (int i=0; i < vec.size() - 1; i++){
                        for (int j=i+1; j < vec.size(); j++){
                            bool can_be_empty = false;
                
                            for (auto& sym : vec[j]->first){
                                if (sym->type == Symbol::empty)
                                    can_be_empty = true;
                                else if (vec[i]->follow.find(sym) == vec[i]->follow.end()){
                                    vec[i]->follow.insert(sym);
                                    is_changed = true;
                                }
                            }
                            
                            if (!can_be_empty)
                                break;
                        }
                    }
                    

                    bool flag = true;
                    
                    for (int i=(int)vec.size()-1; 0 <= i and flag; i--){
                        flag = false;
                        if (vec[i]->type != Symbol::nonterminal)
                            break;
                        
                        for (auto& f: A->follow){
                            if (f->type == Symbol::empty)
                                flag = true;
                            
                            else if (vec[i]->follow.find(f) == vec[i]->follow.end()){
                                vec[i]->follow.insert(f);
                                is_changed = true;
                            }
                        }
                    }
                }
            }
        }
    }
}

void Grammar::create_dfa(){
    Symbol* dot = get_symbol(".");
    Symbol* comma = get_symbol(",");
    Symbol* dollar = get_symbol("$");
    
    std::vector<Symbol*> transition;
    transition.push_back(start);
    transition.push_back(dot);
    
    for (auto& it : *start->transitions.begin()){
        if (it->type == it->end)
            break;
        transition.push_back(it);
    }
    
    transition.push_back(comma);
    transition.push_back(dollar);
    
    State* state = new State(transition);
    for (auto& it : *state->transitions.begin()){
        std::cout << it->name << " ";
    }
    std::cout << std::endl;
}

void Grammar::display(){
    std::cout << "Start: " << start->name << std::endl;
    
    calculate_first_sets();
    calculate_follow_sets();
    
    for (auto& it : symbols){
        if (it.second->type != Symbol::nonterminal)
            continue;
        
        std::cout << it.first << std::endl << "\t";
        
        for (auto& vc : it.second->transitions){
            for (auto& ch : vc)
                std::cout << ch->name;
            
            std::cout << std::endl << "\t";
        }
        
        std::cout << "First: { ";

        for (auto& f : it.second->first)
            std::cout << f->name << " ";
        
        std::cout << "}" << std::endl << "\t";
        
        std::cout << "Follow: { ";
        
        for (auto& f : it.second->follow)
            std::cout << f->name << " ";
        
        std::cout << "}" << std::endl;
    }
    
    std::cout << get_symbol("$")->type << std::endl;
    create_dfa();
}
