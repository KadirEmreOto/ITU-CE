/**************************************

* Student1 Number	: 150120059 Cemal Fatih Kuyucu
* Student2 Number	: 150130903 Meral Korkmaz
* Student3 Number	: 150120004 Cagri Gokce

* Course			: BLG456E
* CRN				: 11792
* Term				: 2015 - 2016 fall
* Project			: Term Project
* File				: "controller.cpp"
***************************************/

#include <stdio.h>
#include <iostream>
#include <conio.h>

#include "tserial.h"	// Necessary example class for serial communication on Windows

using namespace std;

void Menu_Print();		// Menu list function
bool Do_It(char);		// Choose operation function
void Manuel_Control();	// Manuel control function


Tserial *com;			// Serial is a class type defined in these files, used for referring to the communication device

void main() 
{


	bool Terminate = false;		// Terminate Program
	char Menu;					// Menu Item number

	while (!Terminate) 
	{
		Menu_Print();
		cin >> Menu;
		Terminate = Do_It(Menu);
	}
} 

void Menu_Print()
{
	//system("cls");
	//cout << endl << endl;
	cout << "## Taret Control Interface ##" << endl;
	cout << "Choose an operation" << endl;
	cout << "1: Automatic target lockon" << endl;
	cout << "2: Manuel target lockon" << endl;
	cout << "E: Exit" << endl;
	cout << endl;
	cout << "Enter an option {1, 2, E} : ";
}

bool Do_It(char Menu)
{	
	bool Terminate = false;
	
	switch (Menu) 
	{
		case '1':
			system("cls");
			cout << "###################### Output ##############################" << endl << endl;
			cout << "This system cannot using now!!" << endl;
			cout << endl << "############################################################" << endl << endl;
			break; 
		case '2':
			system("cls");
			cout << "###################### Output ##############################" << endl << endl;
			Manuel_Control();
			cout << endl << "############################################################" << endl << endl;
			break;
		case 'E': case 'e':
			cout << "Are you sure that you want to terminate the program? (Y/N):";
			cin >> Menu;
			if (Menu == 'Y' || Menu == 'y'){
				Terminate = true;
			}
			system("cls");
			break;
		default:
			cout << "Error: You have made an invalid choice" << endl;
			cout << "Try again {1, 2, E} :";
			cin >> Menu;
			Terminate = Do_It(Menu);
			break;
	}
	return Terminate;
}

void Manuel_Control()
{
	char command[3] = {0};	// Store the commands for send

	com = new Tserial();	// Define a serial comminication object

	if (com->connect("COM3", 9600, spNONE))	// Start the serial communication with Arduino
	{
		cout << "Serial Port Not Connected." << endl;
		return;
	}
	else
		cout << "Serial Port Connected." << endl;

	while (command[0] != 'E' && command[1] != 'E')	// We get commands from user
	{
		cout << endl << "Enter command to be sent for (Horizontal and Vertical). First char for horizontal, second char for vertical orientation." << endl << "You can enter 'R(Right), L(Left), C(Center)' Example: RR, RC, LC: "; //User prompt
		scanf("%s", command); //User input


		com->sendArray(command, 2);		// Send commands to Arduino
	}
	


	com->disconnect();					//The device is closed down
	delete com;
}