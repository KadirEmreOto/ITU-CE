/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Assignment: 3
 * Lecture: Object Oriented Programming (BLG252E)
 */

#include "cookie.h"

Cookie::Cookie(string pi, double pc, double pr){
    name = pi;
    amount = pc;
    price = (pr / 12);
}

std::ostream& operator<< (std::ostream& os, const Cookie& it){
    os << it.name << " #" << it.amount << " Cost: " << it.price * it.amount;
    return os;
}

string Cookie::get_name() const{
    return name;
}

double Cookie::get_amount() const{
    return amount;
}

double Cookie::get_price() const{
    return price;
}
double Cookie::get_cost() const{
    return price * amount;
}
