// Lab4-OpenCV.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include<math.h>  
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <list>

using namespace cv;
using namespace std;
RNG rng(12345);

void FindAndCountContours(Mat img)
{
	int thresh = 47;
	
	Mat src_gray, canny_output;

	cvtColor(img, src_gray, COLOR_BGR2GRAY);
	blur(src_gray, src_gray, Size(3, 3));
	Canny(src_gray, canny_output, thresh, thresh * 2);
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	findContours(canny_output, contours, hierarchy, 3 ,CHAIN_APPROX_SIMPLE);
	cout << "Counted contours: " << contours.size() << endl;
	Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
	for (size_t i = 0; i < contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
		drawContours(drawing, contours, (int)i, color, 2, LINE_8, hierarchy, 0);
	}
	imshow("Contours task 1", drawing);
}
void HoughLines(Mat img)
{
	Mat canny, img_gray, cdstP;
	// Edge detection
	Canny(img, canny, 50, 200, 3);
	// Copy edges to the images that will display the results in BGR
	cvtColor(canny, img_gray, COLOR_GRAY2BGR);
	cdstP = img_gray.clone();
	// Probabilistic Line Transform
	vector<Vec4i> linesP; // will hold the results of the detection
	HoughLinesP(canny, linesP, 1, CV_PI / 180, 50, 50, 10); // runs the actual detection
	// Draw the lines
	for (size_t i = 0; i < linesP.size(); i++)
	{
		Vec4i l = linesP[i];
		line(img, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, LINE_AA);
	}
	// Show results
	imshow("Straight lines task 2", img);
}
float Slope(float x1, float y1, float x2, float y2)
{
	if (x2 - x1 != 0)
		return (y2 - y1) / (x2 - x1);
	return INT_MAX;
}
#define PI 3.14159265
int FindAngle(double M1, double M2)
{
	// Store the tan value  of the angle
	double angle = abs((M2 - M1)
		/ (1 + M1 * M2));

	// Calculate tan inverse of the angle
	double ret = atan(angle);

	// Convert the angle from
	// radian to degree
	double val = (ret * 180) / PI;

	// Print the result
	return val;
}
Mat HoughLinesParallel(Mat img)
{
	int slope_temp;
	vector<int> list_of_line_slopes;
	vector<Scalar> list_of_colors;
	Mat canny, img_gray, cdstP;
	// Edge detection
	Canny(img, canny, 50, 200, 3);
	// Copy edges to the images that will display the results in BGR
	cvtColor(canny, img_gray, COLOR_GRAY2BGR);
	cdstP = img_gray.clone();
	// Probabilistic Line Transform
	vector<Vec4i> linesP; // will hold the results of the detection
	
	HoughLinesP(canny, linesP, 1, CV_PI / 180, 50, 50, 10); // runs the actual detection
	// Draw the lines
	for (size_t i = 0; i < linesP.size(); i++)
	{
		Vec4i l = linesP[i];
		int slope_temp = Slope(l[0], l[1], l[2], l[3]);
		if (list_of_line_slopes.size() == NULL)
		{
			Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
			list_of_colors.push_back(color);
			line(img, Point(l[0], l[1]), Point(l[2], l[3]), color, 3, LINE_AA);
		}
		else {
			for (int j = 0; j < list_of_line_slopes.size(); j++)
			{
				//int slope_inner = Slope(l[0], l[1], l[2], l[3]);

				if (list_of_line_slopes[j] == slope_temp)
				{
					
					line(img, Point(l[0], l[1]), Point(l[2], l[3]), list_of_colors[j], 3, LINE_AA);
					break;
					//int angle = round(FindAngle(0, list_of_line_slopes[i]));
					//cout << " angle of sheet: " << angle << endl;
				}
				else
				{
					Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
					list_of_colors.push_back(color);
					line(img, Point(l[0], l[1]), Point(l[2], l[3]), color, 3, LINE_AA);
				}
			}
		}
		list_of_line_slopes.push_back( slope_temp);
	}
	// Show results
	return img;
}
Mat HoughLinesAngle(Mat img)
{
	int slope_temp;
	vector<int> list_of_line_slopes;
	//vector<Scalar> list_of_colors;
	Mat canny, img_gray, cdstP;
	// Edge detection
	Canny(img, canny, 50, 200, 3);
	// Copy edges to the images that will display the results in BGR
	cvtColor(canny, img_gray, COLOR_GRAY2BGR);
	cdstP = img_gray.clone();
	// Probabilistic Line Transform
	vector<Vec4i> linesP; // will hold the results of the detection

	HoughLinesP(canny, linesP, 1, CV_PI / 180, 50, 50, 10); // runs the actual detection
	// Draw the lines
	for (size_t i = 0; i < linesP.size(); i++)
	{
		Vec4i l = linesP[i];
		int slope_temp = Slope(l[0], l[1], l[2], l[3]);
		list_of_line_slopes.push_back(slope_temp);
		for (int j = 0; j < list_of_line_slopes.size(); j++)
		{
				//cout << round(FindAngle(list_of_line_slopes[j], slope_temp)) << endl;
			if (round(FindAngle(list_of_line_slopes[j],0)) != 0)
			{
				int angle = round(FindAngle(0, list_of_line_slopes[j]));
				line(img, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0,0,255), 3, LINE_AA);

				cout << " angle of sheet: " << angle << endl;
				break;
			}
		}
	
	}
	// Show results
	return img;
}
Mat HoughCircles(Mat img, int range)
{
    Mat gray, image;
    image = img.clone();

    cvtColor(image, gray, COLOR_BGR2GRAY);

    // Smoothing the image to get rid of noise, so no false detections
    // Here we used a 11x11 filter
    medianBlur(gray, gray, 5);
	vector<Vec3f> circles;

	HoughCircles(gray, circles, HOUGH_GRADIENT, 1,
		100, 100, range, 0, 1000 // change the last two parameters
	); // (min_radius & max_radius) to detect larger circles
	for (size_t i = 0; i < circles.size(); i++)
	{
		Vec3i c = circles[i];
		Point center = Point(c[0], c[1]);
		// circle center
		circle(image, center, 1, Scalar(0, 100, 100), 3, LINE_AA);
		// circle outline
		int radius = c[2];
			circle(image, center, radius, Scalar(255, 0, 255), 3, LINE_AA);
	}
	return image;
}

Mat LoadImage(string task_img)
{
	Mat src = imread(task_img, 1);
	if (src.empty())
	{
		cout << "Could not open or find the image!\n" << endl;
	}
	else {
		resize(src, src, { 500,500 }, 0, 0, INTER_AREA);
		//namedWindow(task_img);
		//imshow(task_img, src); 
		
		// show starting images
		return src;
	}
}

int main()
{
	string task1 = "img1.jpg";
	string task2 = "img2.jpg";
	string task3 = "img3.jpg";
	string task31 = "img4.jpg";
	string task4 = "img5.jpg";
	string task5 = "img6.jpg";
	string task51 = "img7.jpg";

//-------- задание 1. посчитать контуры на изображении
	//FindAndCountContours(LoadImage(task1));	
	//FindAndCountContours(LoadImage(task3));
//-------- задание 2. найти прямые линии при помощи преобразований Хафа
	//HoughLines(LoadImage(task2));
//-------- задание 3. найдите окружности при помощи преобразований Хафа 
		//imshow(task31, HoughCircles(LoadImage(task31), 60)  );
		//imshow(task3, HoughCircles(LoadImage(task3), 100)  );
//-------- 4 *** Найдите на изображении параллельные линии, выделите их другим цветом.
	//imshow(task4, HoughLinesParallel(LoadImage(task4))  );
	//imshow(task5, HoughLinesParallel(LoadImage(task5))  );
//-------- 5 *** Сфотографируйте лист бумаги на столе, лежащий под наклоном, определите его угол наклона.
	imshow(task5,  HoughLinesAngle(LoadImage(task5))  );
	imshow(task51, HoughLinesAngle(LoadImage(task51))  );
	waitKey();

	return 0;
}