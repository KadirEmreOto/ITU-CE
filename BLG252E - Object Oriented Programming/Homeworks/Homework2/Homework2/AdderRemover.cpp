/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Homework #2: Adder and Remover
 */

#include "AdderRemover.hpp"

AdderRemover::AdderRemover(){       // constructor of AddRemover Base Class
    head = tail = NULL;
    traverser = NULL;
    
    nodeCount = 0;
}

AdderRemover::~AdderRemover(){      // destructor of AddRemover Base Class
    if (traverser != NULL)          // if traverser exists, erase it from memory
        delete traverser;
    traverser = NULL;
}

void AdderRemover::display(){
    cout << name << " | NodeCount:" << nodeCount << endl;
    cout << "------" << endl;
    
    if (nodeCount == 0)
        cout << "There is no element to print" << endl;
    
    for (Node* walk = head; walk != NULL; walk = walk->next)    // traverse from begining to end
        cout << walk->value << endl;
}

void AdderRemover::removeAll(){
    while (nodeCount > 0)                                       // while node count is bigger than 0
        remove();                                               // remove a node 
}

void AdderRemover::traverse(){
    cout << name << " | NodeCount:" << nodeCount << endl;
    cout << "------" << endl;
    
    if (!traverser->hasNode())
        cout << "There is no element to traverse" << endl;
    
    while (traverser->hasNode())                                // while traverser has node to traverse
        cout << traverser->next()->value << endl;               // print it and advance
}
