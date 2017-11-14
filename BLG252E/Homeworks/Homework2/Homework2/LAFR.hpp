/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Homework #2: Adder and Remover
 */

#ifndef LAFR_hpp
#define LAFR_hpp

#include <stdio.h>
#include "AdderRemover.hpp"

class LAFR: public AdderRemover {
    
public:
    LAFR();
    ~LAFR();
    
    void add(int value);
    void remove();
    void setTraverser();
};

#endif /* LAFR_hpp */
