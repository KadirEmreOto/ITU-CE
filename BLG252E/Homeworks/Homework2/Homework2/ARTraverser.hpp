/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Homework #2: Adder and Remover
 */

#ifndef ARTraverser_hpp
#define ARTraverser_hpp

#include <stdio.h>
#include "Node.hpp"

class ARTraverser {
    
protected:
    Node* current;
    
public:
    ARTraverser(Node*);
    virtual ~ARTraverser();
    
    virtual bool hasNode()=0;
    virtual Node* next()=0;
};


class ARTF: public ARTraverser {
public:
    ARTF(Node*);
    
    bool hasNode();
    Node* next();
};

class ARTL: public ARTraverser {
public:
    ARTL(Node*);
    
    bool hasNode();
    Node* next();
};

#endif /* ARTraverser_hpp */
