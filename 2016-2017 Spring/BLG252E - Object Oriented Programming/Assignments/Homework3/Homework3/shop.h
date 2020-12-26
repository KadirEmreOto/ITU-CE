/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Assignment: 3
 * Lecture: Object Oriented Programming (BLG252E)
 */

#ifndef shop_h
#define shop_h

#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>
#include "cookie.h"
#include "candy.h"
#include "icecream.h"

using namespace std;

template <class T> class Shop;
template <class T> ostream& operator<<(ostream& os, const Shop<T>& a);              // for template friend function

template <class food>
class Shop {
    int tax;
    double discount;

    vector<food> items;

public:
    Shop();
    Shop(food f);
    ~Shop();

    int size();                                                                     // returns item count
    void add(food f);
    void setDiscount(double d);
    food& operator[](int index);
    friend std::ostream& operator<< <>(std::ostream& os, const Shop<food>& shop);
};


template<class food>
std::ostream& operator<< (std::ostream& os, const Shop<food>& shop){                // overloading << operator
    os << "**********************" << endl;
    os << "Number of items: " << shop.items.size() << endl;

    double total = 0;
    for (int i=0; i < shop.items.size(); i++){
        os << i+1 << ": " << shop.items[i] << endl;
        total += shop.items[i].get_cost();
    }
    
    os << "**********************" << endl;
    total = total * (100 + shop.tax) / 100;
    os << "Total cost: " << total << endl;


    if (shop.discount){
        os << "Discount: " << shop.discount << "%" << endl;
        os << "Discount amount: -" << total * shop.discount / 100 << endl;
        os << "Discounted cost: " << total * (100 - shop.discount) / 100 << endl;
    }

    os << "**********************" << endl;
    return os;
}

#endif /* shop_h */
