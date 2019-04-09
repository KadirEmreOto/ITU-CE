/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Homework #2: Adder and Remover
 */

#include "LALR.hpp"

LALR::LALR(){
    name = "LALR";
}

LALR::~LALR(){
    removeAll();
}

void LALR::add(int value){
    if (head == NULL)                   // if head is null, it means list is empty
        head = tail = new Node(value);  // head and tail will be new element
    
    else{
        tail->next = new Node(value);   // create new element
        tail->next->prev = tail;
        tail = tail->next;              // set it new tail
    }
    nodeCount++;
}

void LALR::remove(){                    // removes from the end of the list (tail)
    Node* toRemove = tail;              // holds a copy of pointer of the tail
    
    if (head == tail)                   // if list have only one element
        head = tail = NULL;             // assigns head and tail to null
    
    else{
        tail = tail->prev;              // set new tail
        tail->next = NULL;
    }
    
    delete toRemove;                    // delete from memory
    nodeCount--;
}

void LALR::setTraverser(){
    if (traverser == NULL)
        traverser = new ARTL(tail);
}
