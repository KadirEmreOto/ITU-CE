/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Assignment: 3
 * Lecture: Object Oriented Programming (BLG252E)
 */

#ifndef cookie_h
#define cookie_h

#include <iostream>
#include <stdio.h>
#include <string>
#include "shop.h"

using namespace std;

class Cookie {
    string name;
    double amount;
    double price;
    
public:
    Cookie(string s, double pi, double pr);
    friend std::ostream& operator<< (std::ostream& os, const Cookie& it);
    
    string get_name() const;
    double get_amount() const;
    double get_price() const;
    double get_cost() const;
};



#endif /* cookie_h */
