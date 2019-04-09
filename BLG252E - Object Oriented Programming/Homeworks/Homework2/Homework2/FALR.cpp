/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Homework #2: Adder and Remover
 */

#include "FALR.hpp"

FALR::FALR(){
    name = "FALR";
}

FALR::~FALR(){
    removeAll();
}

void FALR::add(int value){              // adds to the beginning of the list
    if (head == NULL)                   // if the list is empty
        head = tail = new Node(value);  // new node should be head and tail
    
    else{
        Node* toAdd = new Node(value);  // create new node to add
        toAdd->next = head;
        head->prev = toAdd;
        head = toAdd;                   // new head is now toAdd
    }
    nodeCount++;
}

void FALR::remove(){                    // removes from the end of the list (tail)
    Node* toRemove = tail;              // holds a copy of pointer of the tail
    
    if (head == tail)                   // if list have only one element
        head = tail = NULL;             // assigns head and tail to null
    
    else{
        tail = tail->prev;              // set new tail
        tail->next = NULL;
    }
    
    delete toRemove;                    // delete from memory
    nodeCount--;}

void FALR::setTraverser(){
    if (traverser == NULL)
        traverser = new ARTF(head);
}
