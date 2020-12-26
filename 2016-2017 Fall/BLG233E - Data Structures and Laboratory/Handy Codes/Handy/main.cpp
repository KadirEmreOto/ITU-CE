//
//  main.cpp
//  Handy
//
//  Created by Kadir Emre Oto on 24/12/2016.
//  Copyright © 2016 Kadir Emre Oto. All rights reserved.
//

#include <cstdlib>
#include <iostream>

using namespace std;

struct vector{
    int cursor;
    int maxsize;
    int* array;
    
    vector(){
        cursor = 0;
        maxsize = 2;
        array = new int[maxsize];
    }
    
    void push_back(int item){
        if (cursor == maxsize){
            maxsize <<= 1;
    
            int* temp = new int[maxsize];
            for (int i=0; i < cursor; i++) temp[i] = array[i];
            
            delete [] array;
            array = temp;
        }
        
        array[cursor++] = item;
    }
    
    int pop(){
        if (cursor == (maxsize >> 1)){
            maxsize >>= 1;
            
            int* temp = new int[maxsize];
            for (int i=0; i < cursor; i++) temp[i] = array[i];
            
            delete [] array;
            array = temp;
        }
        
        return array[--cursor];
    }
    
    int length(){
        return cursor;
    }
    
    int at(int index){
        return array[index];
    }
};

struct node {
    int data;
    node* next;
    
    node(const int c){
        data = c;
        next = NULL;
    }
};

struct stack{
    int length;
    node* head;
    
    stack(){
        head = NULL;
        length = 0;
    }
    
    void push(node* t){
        t->next = head;
        head = t;
        length++;
    }
    
    node* pop(){
        node* r = head;
        head = head->next;
        length--;
        return r;
    }
    
    bool is_empty(){
        return head == NULL;
    }
};

struct queue{
    int length;
    node* head;
    
    queue(){
        length = 0;
        head = NULL;
    }
    
    void push(node* t){
        node** indirect = &head;
        
        while (*indirect)
            indirect = &(*indirect)->next;
        
        *indirect = t;
    }
    
    node* pop(){
        node* r = head;
        head = head->next;
        length--;
        return r;
    }
    
    bool is_empty(){
        return head == NULL;
    }
};


int main(int argc, const char * argv[]) {
    vector* v = new vector;
    
    for (int i = 0; i < 10; i++)
        v->push_back(i);
    
    while (v->length())
        cout << v->pop() << endl;
    
    return 0;
}
