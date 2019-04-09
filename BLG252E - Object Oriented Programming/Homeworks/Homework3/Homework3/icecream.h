/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Assignment: 3
 * Lecture: Object Oriented Programming (BLG252E)
 */

#ifndef icecream_h
#define icecream_h

#include <iostream>
#include <stdio.h>
#include <string>

using namespace std;

class Icecream {
    string name;
    double amount;
    double price;
    
public:
    Icecream(string s, double pi, double pr);
    friend std::ostream& operator<< (std::ostream& os, const Icecream& it);
    
    string get_name() const;
    double get_amount() const;
    double get_price() const;
    double get_cost() const;
};

#endif /* icecream_h */
