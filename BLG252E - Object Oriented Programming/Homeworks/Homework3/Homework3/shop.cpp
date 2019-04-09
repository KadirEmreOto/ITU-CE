/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Assignment: 3
 * Lecture: Object Oriented Programming (BLG252E)
 */

#include "shop.h"

template<class food>
Shop<food>::Shop(){
    tax = 8;
    discount = 0;
}

template<class food>
Shop<food>::Shop(food f){
    tax = 8;
    discount = 0;
    add(f);
}

template<class food>
Shop<food>::~Shop(){
    items.clear();
}

template<class food>
void Shop<food>::add(food f){
    items.push_back(f);
}

template<class food>
void Shop<food>::setDiscount(double d){
    if (0 <= d && d <= 30)
        discount = d;
    
    else
        throw "Discount rate is out of range!";
}

template<class food>
food& Shop<food>::operator[](int index){
    if (index >= items.size())
        throw "We don't have enough cookies!";
    
    return items[index];
}

template <class food>
int Shop<food>::size() {
    return (int)items.size();
}

template class Shop<Cookie>;
template class Shop<Candy>;
template class Shop<Icecream>;

