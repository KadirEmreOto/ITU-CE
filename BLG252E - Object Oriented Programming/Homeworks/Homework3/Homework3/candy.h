/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Assignment: 3
 * Lecture: Object Oriented Programming (BLG252E)
 */

#ifndef candy_h
#define candy_h

#include <iostream>
#include <stdio.h>
#include <string>


using namespace std;

class Candy {
    string name;
    double amount;
    double price;
    
public:
    Candy(string s, double pi, double pr);
    friend std::ostream& operator<< (std::ostream& os, const Candy& it);
    
    string get_name() const;
    double get_amount() const;
    double get_price() const;
    double get_cost() const;
};

#endif /* candy_h */
