#include <iostream>
#include <cstdlib>
#include <string>
#include "shop.h"
#include "candy.h"
#include "cookie.h"
#include "icecream.h"
using namespace std;


int main() {
    Cookie cookie1("Chocolate Chip Cookies",10, 180); //(name, pieces, priceperdozen)
	Cookie cookie2("Cake Mix Cookies", 16, 210);
	
	Shop<Cookie> cookieShop(cookie1);
	cookieShop.add(cookie2);
	cout<<cookieShop<<endl;

	//Catch an exception here while trying to access the element at(2)
    cout << cookieShop[2] << endl;

	Icecream icecream1("Chocolate ice cream",1.5, 170); //(name, litre, priceperlitre)
	Shop<Icecream> icecreamShop(icecream1); 
	cout<<icecreamShop<<endl;

	//Catch an exception here while trying to set the discount to(50)       
	icecreamShop.setDiscount(50);

	Candy candy2("Gummi bears",12,89); //(name, weight, priceperkg)
	Candy candy3("Hanukkah gelt",8,110);

	Shop<Candy> candyShop(candy2);
	candyShop.add(candy3);
    
    candyShop.setDiscount(15);
    cout<<candyShop<<endl;
	
/*You will also
1) Read dessert stock and customer order from "stock.txt" and "order.txt", respectively.
2) Write the total cost with 10% discount to "checkout.txt".
Expected output for checkout:
!!! We don't have 7 (Orange Jelly Candy)s.
!!! We don't have 11 (Chocolate ice cream)s.
-----------------------
Number of items:4
1: Chocolate Chip Cookies #2 Cost: 100
2: Orange Jelly Candy #5 Cost: 750
3: Chocolate ice cream #2 Cost: 480
4: Cake Mix Cookies #10 Cost: 150
**********************
Total cost: 1598.40
Discount: %10
Discount amount: -159.84
Discounted cost: 1438.56
**********************
*/