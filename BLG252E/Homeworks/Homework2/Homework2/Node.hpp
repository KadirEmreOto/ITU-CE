/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Homework #2: Adder and Remover
 */

#ifndef Node_hpp
#define Node_hpp

#include <stdio.h>

class Node {
    
public:
    Node(int x);
    
    Node* next;
    Node* prev;
    
    int value;
};

#endif /* Node_hpp */
