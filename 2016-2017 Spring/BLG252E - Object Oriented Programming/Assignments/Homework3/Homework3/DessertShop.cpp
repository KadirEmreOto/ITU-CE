/*
 * Author: Kadir Emre Oto
 * Student ID: 150140032
 * Assignment: 3
 * Lecture: Object Oriented Programming (BLG252E)
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>

#include "shop.h"
#include "candy.h"
#include "cookie.h"
#include "icecream.h"

using namespace std;

void process(){                                             // file operations
    int type;
    char name[100];
    double amount, price;
    
    Shop<Candy> candy_shop;
    Shop<Cookie> cookie_shop;
    Shop<Icecream> icecream_shop;
    
    candy_shop.setDiscount(10);
    cookie_shop.setDiscount(10);
    icecream_shop.setDiscount(10);
    
    FILE* stock = fopen("stock.txt", "r");
    
    fscanf(stock, "%[^\n]s", name);                         // read the first line (header line)
    fgetc(stock);                                           // read the new line (\n) character
    
    while (!feof(stock)){
        fscanf(stock, "%[^\t]s", name);                     // read the name
        if (feof(stock)) break;
        fgetc(stock);                                       // read the tab (\t) character
        
        fscanf(stock, "%d", &type);                         // read the name
        fscanf(stock, "%lf", &amount);                      // read the name
        fscanf(stock, "%lf ", &price);                      // read the name
        
        if (type == 1){
            Cookie item(name, amount, price);
            cookie_shop.add(item);
        }
        
        else if (type == 2){
            Candy item(name, amount, price);
            candy_shop.add(item);
        }
        
        else{
            Icecream item(name, amount, price);
            icecream_shop.add(item);
        }
    }
    
    
    FILE* order = fopen("order.txt", "r");
    ofstream checkout("checkout.txt");
    
    Shop<Icecream> generic;                                 // suppose a generic shop exist - convert all items to icecream
    generic.setDiscount(10);                                
    
    while (!feof(order)){
        fscanf(order, "%[^\t]s", name);                     // read the name
        if (feof(order)) break;
        fgetc(order);                                       // read the tab (\t) character
        
        fscanf(order, "%d", &type);                         // read the name
        fscanf(order, "%lf ", &amount);                     // read the name
        
        
        if (type == 1){
            for (int i=0; i < cookie_shop.size(); i++){     // find cookie
                if (cookie_shop[i].get_name() == name){
                    if (cookie_shop[i].get_amount() < amount)
                        checkout << "!!! We don't have " << amount << " (" << name <<")s.\n";

                    Icecream item(name, min(amount, cookie_shop[i].get_amount()), cookie_shop[i].get_price());
                    generic.add(item);
                    break;
                }
            }
        }
        
        else if (type == 2){
            for (int i=0; i < candy_shop.size(); i++){
                if (candy_shop[i].get_name() == name){      // find candy
                    if (candy_shop[i].get_amount() < amount)
                        checkout << "!!! We don't have " << amount << " (" << name <<")s.\n";
                    
                    Icecream item(name, min(amount, candy_shop[i].get_amount()), candy_shop[i].get_price());
                    generic.add(item);
                    break;
                }
            }
        }
        
        else{
            for (int i=0; i < icecream_shop.size(); i++){
                if (icecream_shop[i].get_name() == name){   // find icecream
                    if (icecream_shop[i].get_amount() < amount)
                        checkout << "!!! We don't have " << amount << " (" << name <<")s.\n";
                    
                    Icecream item(name, min(amount, icecream_shop[i].get_amount()), icecream_shop[i].get_price());
                    generic.add(item);
                    break;
                }
            }
        }
    }
    
    checkout << generic << endl;                            // write items to file
}

int main(int argc, const char * argv[]) {
    Cookie cookie1("Chocolate Chip Cookies", 10, 180);      //(name, pieces, priceperdozen)
    Cookie cookie2("Cake Mix Cookies", 16, 210);
    
    Shop<Cookie> cookieShop(cookie1);
    cookieShop.add(cookie2);
    cout<<cookieShop<<endl;
    
    try {                                                   //Catch an exception here while trying to access the element at(2)
        cout << cookieShop[2] << endl;
    } catch (const char* e) {
        cout << e << endl;
    }
    
    Icecream icecream1("Chocolate ice cream", 1.5, 170);    //(name, litre, priceperlitre)
    Shop<Icecream> icecreamShop(icecream1);
    cout<<icecreamShop<<endl;
    
    try {                                                   //Catch an exception here while trying to set the discount to(50)
        icecreamShop.setDiscount(50);
    } catch (const char* e) {
        cout << e << endl;
    }
    
    Candy candy2("Gummi bears", 12, 89);                    //(name, weight, priceperkg)
    Candy candy3("Hanukkah gelt", 8, 110);
    
    Shop<Candy> candyShop(candy2);
    candyShop.add(candy3);
    
    candyShop.setDiscount(15);
    cout<<candyShop<<endl;
    
    process();                                              // for file operations
    
    return 0;
}
