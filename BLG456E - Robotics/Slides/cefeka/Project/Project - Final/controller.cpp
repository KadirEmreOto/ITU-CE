/**************************************

* Student1 Number	: 150120059 Cemal Fatih Kuyucu
* Student2 Number	: 150130903 Meral Korkmaz
* Student3 Number	: 150120004 Çaðrý Gökçe

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

#include"opencv2/core/core.hpp"			// OpencCV libraries
#include"opencv2/highgui/highgui.hpp"
#include"opencv2/imgproc/imgproc.hpp"
#include"opencv2/photo/photo.hpp"

using namespace std;
using namespace cv;

void Menu_Print();		// Menu list function
bool Do_It(char);		// Choose operation function
void Manuel_Control();	// Manuel control function
void Automatic_Control(); // Automatic control function

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
			Automatic_Control();
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


void Automatic_Control()
{
	char command[3] = { 0 };	// Store the commands for send

	com = new Tserial();	// Define a serial comminication object

	if (com->connect("COM3", 9600, spNONE))	// Start the serial communication with Arduino
	{
		cout << "Serial Port Not Connected." << endl;
		//return;
	}
	else
		cout << "Serial Port Connected." << endl;

	

	cv::VideoCapture capWebcam(0);		// declare a VideoCapture object and associate to webcam, 0 => use 1st webcam

	if (capWebcam.isOpened() == false) {				// check if VideoCapture object was associated to webcam successfully
		std::cout << "error: capWebcam not accessed successfully\n\n";	// if not, print error message to std out
		return;														// and exit program
	}
	int sayac = 0;
	int spamsayac = 0;
	cv::Mat matOriginal;		// input image
	cv::Mat matProcessed;		// output image
	cv::Mat matProcessed2;		// output image2

	std::vector<cv::Vec3f> v3fCircles;				// 3 element vector of floats, this will be the pass by reference output of HoughCircles()
	std::vector<cv::Vec3f> v3fCircles2;				// vol2
	char charCheckForEscKey = 0;

	while (charCheckForEscKey != 27 && capWebcam.isOpened()) {		// until the Esc key is pressed or webcam connection is lost
		bool blnFrameReadSuccessfully = capWebcam.read(matOriginal);		// get next frame

		if (!blnFrameReadSuccessfully || matOriginal.empty()) {		// if frame not re                         ad successfully
			std::cout << "error: frame not read from webcam\n";		// print error message to std out
			break;													// and jump out of while loop
		}
		// smooth the image
		cv::GaussianBlur(matOriginal,			// function input
			matProcessed,						// function output
			cv::Size(5, 5),						// smoothing window width and height in pixels
			2);									// sigma value, determines how much the image will be blurred

		// vol 2.  smooth the image
		cv::GaussianBlur(matOriginal,			// function input
			matProcessed2,						// function output
			cv::Size(5, 5),						// smoothing window width and height in pixels
			2);







		// filter on color
		cv::inRange(matProcessed,				// funcion input
			cv::Scalar(180, 180, 220),				// min filtering value (if greater than or equal to this) (in BGR format)
			cv::Scalar(251, 251, 256),			// max filtering value (and if less than this) (in BGR format)
			matProcessed);						// function output

		// vol 2. filter on color
		cv::inRange(matProcessed2,				// funcion input
			cv::Scalar(0, 0, 0),				// min filtering value (if greater than or equal to this) (in BGR format)
			cv::Scalar(30, 30, 30),			// max filtering value (and if less than this) (in BGR format)
			matProcessed2);

		// smooth again
		cv::GaussianBlur(matProcessed,			// function input
			matProcessed,						// function output
			cv::Size(5, 5),						// smoothing window width and height in pixels
			2);									// sigma value, determines how much the image will be blurred

		// vol 2. smooth
		cv::GaussianBlur(matProcessed2,			// function input
			matProcessed2,						// function output
			cv::Size(5, 5),						// smoothing window width and height in pixels
			2);									// sigma value, determines how much the image will be blurred

		//dilate n erode
		cv::dilate(matProcessed, matProcessed, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));		// close image (dilate, then erode)
		cv::erode(matProcessed, matProcessed, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));		// closing "closes" (i.e. fills in) foreground gaps


		//dilate n erode vol2
		cv::dilate(matProcessed2, matProcessed2, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));		// close image (dilate, then erode)
		cv::erode(matProcessed2, matProcessed2, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5)));		// closing "closes" (i.e. fills in) foreground gaps

		// fill circles vector with all circles in processed image
		cv::HoughCircles(matProcessed,			// input image
			v3fCircles,							// function output (must be a standard template library vector
			CV_HOUGH_GRADIENT,					// two-pass algorithm for detecting circles, this is the only choice available
			2,									// size of image / this value = "accumulator resolution", i.e. accum res = size of image / 2
			matProcessed.rows / 4,				// min distance in pixels between the centers of the detected circles
			100,								// high threshold of Canny edge detector (called by cvHoughCircles)						
			50,									// low threshold of Canny edge detector (set at 1/2 previous value)
			10,									// min circle radius (any circles with smaller radius will not be returned)
			400);								// max circle radius (any circles with larger radius will not be returned)



		// vol 2 fill circles vector with all circles in processed image
		cv::HoughCircles(matProcessed2,			// input image
			v3fCircles2,						// function output (must be a standard template library vector
			CV_HOUGH_GRADIENT,					// two-pass algorithm for detecting circles, this is the only choice available
			2,									// size of image / this value = "accumulator resolution", i.e. accum res = size of image / 2
			matProcessed2.rows / 4,				// min distance in pixels between the centers of the detected circles
			100,								// high threshold of Canny edge detector (called by cvHoughCircles)						
			50,									// low threshold of Canny edge detector (set at 1/2 previous value)
			10,									// min circle radius (any circles with smaller radius will not be returned)
			400);								// max circle radius (any circles with larger radius will not be returned)

		for (int i = 0; i < v3fCircles.size(); i++) {		// for each circle . . .
			// show ball position x, y, and radius to command line
			//std::cout << "laser position x = " << v3fCircles[i][0]			// x position of center point of circle
			//	<< ", y = " << v3fCircles[i][1]								// y position of center point of circle
			//	<< ", radius = " << v3fCircles[i][2] << "\n";				// radius of circle

			//std::cout << i;
			// draw small green circle at center of detected object
			cv::circle(matOriginal,												// draw on original image
				cv::Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]),		// center point of circle
				3,																// radius of circle in pixels
				cv::Scalar(0, 255, 0),											// draw pure green (remember, its BGR, not RGB)
				CV_FILLED);														// thickness, fill in the circle




			// draw red circle around the detected object
			cv::circle(matOriginal,												// draw on original image
				cv::Point((int)v3fCircles[i][0], (int)v3fCircles[i][1]),		// center point of circle
				(int)v3fCircles[i][2],											// radius of circle in pixels
				cv::Scalar(0, 0, 255),											// draw pure red (remember, its BGR, not RGB)
				3);																// thickness of circle in pixels



		}	// end for

		for (int i = 0; i < v3fCircles2.size(); i++) {		// for each circle . . .

			//std::cout << i;

			// vol2. show ball position x, y, and radius to command line
			//std::cout << "ball position x = " << v3fCircles2[i][0]			// x position of center point of circle
			//	<< ", y = " << v3fCircles2[i][1]								// y position of center point of circle
			//	<< ", radius = " << v3fCircles2[i][2] << "\n";				// radius of circle


			// vol 2 draw small green circle at center of detected object
			cv::circle(matOriginal,												// draw on original image
				cv::Point((int)v3fCircles2[i][0], (int)v3fCircles2[i][1]),		// center point of circle
				3,																// radius of circle in pixels
				cv::Scalar(0, 255, 0),											// draw pure green (remember, its BGR, not RGB)
				CV_FILLED);
			// thickness of circle in pixels
			// vol 2 draw red circle around the detected object
			cv::circle(matOriginal,												// draw on original image
				cv::Point((int)v3fCircles2[i][0], (int)v3fCircles2[i][1]),		// center point of circle
				(int)v3fCircles2[i][2],											// radius of circle in pixels
				cv::Scalar(0, 0, 255),											// draw pure red (remember, its BGR, not RGB)
				3);


		}	// end for


		if (v3fCircles.empty() && v3fCircles2.empty()){
			spamsayac--;
			if (spamsayac <= 0){
				std::cout << "Target and laser is not detected \n";
				spamsayac = 5;
			}
		}
		else if (v3fCircles2.empty()){
			std::cout << "Target is not on screen\n";
		}
		else if (v3fCircles.empty()){

			sayac--;
			if (sayac < 0){

				sayac = 0;
			}
			if (sayac == 0){
				//std::cout << "LASER X: " << v3fCircles[0][0] << " Y: " << v3fCircles[0][1] << " HEDEF X: " << v3fCircles2[0][0] << " Y: " << v3fCircles[0][1] << endl;

				std::cout << "Waiting for command or Target reached. \n";
				sayac = 25;
			}
		}
		else{

			std::cout << "LASER X: " << v3fCircles[0][0] << " Y: " << v3fCircles[0][1] << " HEDEF X: " << v3fCircles2[0][0] << " Y: " << v3fCircles[0][1] << endl;

			///////////SERÝAL EKLENECEK KISIM BAÞI
			//BU KISIMDA TOP VE LAZER AYNI ANDA EKRANDA GÖRÜNÜYOR VE KORDÝNATLAR VERÝLÝYOR.
			///v3fCircles[0][1] ---> lazerin y'si
			///v3fCircles2[0][1] ----> hedef'in y'si
			///v3fCircles[0][0] ---> lazerin x'i
			///7v3fCircles2[0][0] ----> hedef'in x'i

			//Commentleri tekrar düzenlerim ben sonra.


			//Laser is at on screen now, making calculations and will send commands to ardunio
			//A small delay could be applied here
			std::cout << "Target is at";
			sayac = 5;
			if ((v3fCircles[0][1] - v3fCircles2[0][1])>0){

				std::cout << "upper ";
				command[1] = 'U';

			}
			else{
				std::cout << "below ";
				command[1] = 'D';
			}

			if ((v3fCircles[0][0] - v3fCircles2[0][0])>0){

				std::cout << "left ";
				command[0] = 'L';

			}
			else{
				std::cout << "right ";
				command[0] = 'R';
			}

			std::cout << "side of the laser.\n";



			com->sendArray(command, 2);		// Send commands to Arduino

			/////SERÝAL EKLENECEK KISIM SONU
		}




		// declare windows
		cv::namedWindow("Original", CV_WINDOW_AUTOSIZE);	// note: you can use CV_WINDOW_NORMAL which allows resizing the window
		cv::namedWindow("Processed", CV_WINDOW_AUTOSIZE);	// or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image
		cv::namedWindow("Processed2", CV_WINDOW_AUTOSIZE);	// vol2 or CV_WINDOW_AUTOSIZE for a fixed size window matching the resolution of the image

		// CV_WINDOW_AUTOSIZE is the default

		cv::imshow("Original", matOriginal);			// show windows
		cv::imshow("Processed", matProcessed);
		cv::imshow("Processed2", matProcessed2);           //vol2

		charCheckForEscKey = cv::waitKey(1);			// delay (in ms) and get key press, if any
	}	// end while




	com->disconnect();					//The device is closed down
	delete com;
}