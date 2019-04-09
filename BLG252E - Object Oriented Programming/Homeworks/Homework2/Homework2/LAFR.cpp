/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Homework #2: Adder and Remover
 */

#include "LAFR.hpp"

LAFR::LAFR(){
    name = "LAFR";
}

LAFR::~LAFR(){
    removeAll();
}

void LAFR::add(int value){
    if (head == NULL)                   // if head is null, it means list is empty
        head = tail = new Node(value);  // head and tail will be new element
    
    else{
        tail->next = new Node(value);   // create new element
        tail->next->prev = tail;
        tail = tail->next;              // set it new tail
    }
    nodeCount++;
}

void LAFR::remove(){
    Node* toRemove = head;              // holds a copy of head pointer
    
    if (head == tail)                   // if the list has only one node
        head = tail = NULL;             // assigns head and tail to null
    
    else{
        head = head->next;              // slides head
        head->prev = NULL;
    }
    
    nodeCount--;
    delete toRemove;                    // deletes the head after sliding
}

void LAFR::setTraverser(){
    if (traverser == NULL)
        traverser = new ARTL(tail);
}
