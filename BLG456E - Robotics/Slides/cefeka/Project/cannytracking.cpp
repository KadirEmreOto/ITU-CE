// RedBallTracker.cpp

/*
ARKAFON: Beyaz (örn. beyaz kaðýt )
HEDEF: Yuvarlak Siyah Nesne (örn. siyah yuvarlak karton, yuvarlak olmasý önemli), el ile gölge olmamasý için beyaz bi çubuk kaðýt vs ucuna tutturulursa iyi olur.
olmalý.
*/



#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

#include<iostream>

///////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
	cv::VideoCapture capWebcam(0);		// declare a VideoCapture object and associate to webcam, 0 => use 1st webcam

	if (capWebcam.isOpened() == false) {				// check if VideoCapture object was associated to webcam successfully
		std::cout << "error: capWebcam not accessed successfully\n\n";	// if not, print error message to std out
		return(0);														// and exit program
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

		if (!blnFrameReadSuccessfully || matOriginal.empty()) {		// if frame not read successfully
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
			cv::Scalar(140, 120, 220),				// min filtering value (if greater than or equal to this) (in BGR format)
			cv::Scalar(180, 160, 256),			// max filtering value (and if less than this) (in BGR format)
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
			v3fCircles2,							// function output (must be a standard template library vector
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
		}else if (v3fCircles2.empty()){
			std::cout << "Target is not on screen\n";
		}
		else if (v3fCircles.empty()){
			
			sayac--;
			if (sayac < 0){

				sayac = 0;
			}
			if (sayac == 0){
				std::cout << "Waiting for command or Target reached. \n";
				sayac = 25;
			}
		}
		else{


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

			}
			else{
				std::cout << "below ";
			}

			if ((v3fCircles[0][0] - v3fCircles2[0][0] )>0){

				std::cout << "left ";

			}
			else{
				std::cout << "right ";
			}
			
			std::cout << "side of the laser.\n";




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

	return(0);
}