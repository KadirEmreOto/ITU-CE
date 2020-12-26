/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Homework #2: Adder and Remover
 */

#include "FAFR.hpp"

FAFR::FAFR(){
    name = "FAFR";
}

FAFR::~FAFR(){
    removeAll();
}

void FAFR::add(int value){              // adds to the beginning of the list
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

void FAFR::remove(){
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

void FAFR::setTraverser(){
    if (traverser == NULL)
        traverser = new ARTF(head);
}
