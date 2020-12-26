/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Homework #2: Adder and Remover
 */

#include <iostream>

#include "FAFR.hpp"
#include "LALR.hpp"
#include "FALR.hpp"
#include "LAFR.hpp"
#include "AdderRemover.hpp"

using namespace std;

int main(int argc, char **argv) {
    
    AdderRemover *myList[4];
    
    myList[0] = new FAFR();
    myList[1] = new LALR();
    myList[2] = new FALR();
    myList[3] = new LAFR();
    
    for(int j=0; j<4; j++) {
        for(int i=0; i<5; i++)
            myList[j]->add(i);
    }
    
    for(int j=0; j<4; j++){
        myList[j]->remove();
        myList[j]->display();
        myList[j]->removeAll();
        myList[j]->display();
    }
    
    cout << endl << "Test case for ARTraverser"<< endl;
    for(int j=0; j<4; j++) {
        for(int i=0; i<5; i++)
            myList[j]->add(i);
        myList[j]->remove();
    }
    
    for(int j=0; j<4; j++){
        myList[j]->setTraverser();
        myList[j]->traverse();
    }
    
    for(int j=0; j<4; j++){
        delete myList[j];
    }
    
    
    return 0;
}
