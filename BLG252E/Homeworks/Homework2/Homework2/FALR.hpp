/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Homework #2: Adder and Remover
 */

#ifndef FALR_hpp
#define FALR_hpp

#include <stdio.h>
#include "AdderRemover.hpp"

class FALR: public AdderRemover {
    
public:
    FALR();
    ~FALR();
    
    void add(int value);
    void remove();
    void setTraverser();
};

#endif /* FALR_hpp */
