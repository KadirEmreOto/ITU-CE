/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Assignment: 3
 * Lecture: Object Oriented Programming (BLG252E)
 */

#include "candy.h"

Candy::Candy(string pi, double pc, double pr){
    name = pi;
    amount = pc;
    price = pr;
}

std::ostream& operator<< (std::ostream& os, const Candy& it){
    os << it.name << " Cost: " << it.price * it.amount;
    return os;
}

string Candy::get_name() const{
    return name;
}

double Candy::get_amount() const{
    return amount;
}

double Candy::get_price() const{
    return price;
}

double Candy::get_cost() const{
    return price * amount;
}

