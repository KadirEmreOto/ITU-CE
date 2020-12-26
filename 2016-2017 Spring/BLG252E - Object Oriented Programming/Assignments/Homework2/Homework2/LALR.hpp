/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Homework #2: Adder and Remover
 */

#ifndef LALR_hpp
#define LALR_hpp

#include <stdio.h>
#include "Node.hpp"
#include "AdderRemover.hpp"

class LALR: public AdderRemover {
    
public:
    LALR();
    ~LALR();
    
    void add(int value);
    void remove();
    void setTraverser();
};


#endif /* LALR_hpp */
