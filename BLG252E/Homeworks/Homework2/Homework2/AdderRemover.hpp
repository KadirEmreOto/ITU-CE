/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Homework #2: Adder and Remover
 */

#ifndef AdderRemover_hpp
#define AdderRemover_hpp

#include <string>
#include <iostream>

#include "Node.hpp"
#include "ARTraverser.hpp"

using namespace std;

class AdderRemover {
    
protected:
    Node* head;
    Node* tail;
    
    int nodeCount;
    string name;
    
    ARTraverser* traverser;
    
public:
    AdderRemover();
    virtual ~AdderRemover();
    
    virtual void add(int value)=0;
    virtual void remove()=0;
    virtual void setTraverser()=0;
    
    virtual void display();
    virtual void removeAll();
    virtual void traverse();
};

#endif /* AdderRemover_hpp */
