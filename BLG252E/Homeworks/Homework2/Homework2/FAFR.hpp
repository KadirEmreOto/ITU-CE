/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Homework #2: Adder and Remover
 */

#ifndef FAFR_hpp
#define FAFR_hpp

#include <stdio.h>
#include "Node.hpp"
#include "AdderRemover.hpp"
#include "ARTraverser.hpp"

class FAFR: public AdderRemover {
    
public:
    FAFR();
    ~FAFR();
    
    void add(int value);
    void remove();
    void setTraverser();
};

#endif /* FAFR_hpp */
