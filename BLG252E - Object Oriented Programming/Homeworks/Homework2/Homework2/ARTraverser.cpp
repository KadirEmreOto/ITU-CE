/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Homework #2: Adder and Remover
 */

#include "ARTraverser.hpp"

ARTraverser::ARTraverser(Node* node){       // constructor of ARTraverser
    current = node;                         // assigns current node
}

ARTraverser::~ARTraverser(){}

ARTF::ARTF(Node* node): ARTraverser(node){} // constructor of ARTF calls ARTraverser constructor
ARTL::ARTL(Node* node): ARTraverser(node){} // constructor of ARTL calls ARTraverser constructor

bool ARTF::hasNode(){
    return current != NULL;
}

Node* ARTF::next(){
    Node* node = current;                   // holds current node
    current = current->next;                // change current node
    return node;                            // return current node
}

bool ARTL::hasNode(){
    return current != NULL;
}

Node* ARTL::next(){
    Node* node = current;                   // holds current node
    current = current->prev;                // change current node
    return node;                            // return current node
}
