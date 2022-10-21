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
using namespace cv;
using namespace std;
Mat src, src_gray;
Mat myHarris_dst, myHarris_copy, Mc;
Mat myShiTomasi_dst, myShiTomasi_copy;
int myShiTomasi_qualityLevel = 11;
int myHarris_qualityLevel = 38;
int max_qualityLevel = 150;
double myHarris_minVal, myHarris_maxVal;
double myShiTomasi_minVal, myShiTomasi_maxVal;
RNG rng(12345);
const char* myHarris_window = "My Harris corner detector";
const char* myShiTomasi_window = "My Shi Tomasi corner detector";
void myShiTomasi_function(int, void*);
void myHarris_function(int, void*);
void affine(Mat);


void myShiTomasi_function(int, void*)
{
    vector<Point> corners;
    myShiTomasi_copy = src.clone();
    myShiTomasi_qualityLevel = MAX(myShiTomasi_qualityLevel, 1);
    goodFeaturesToTrack(src_gray, corners, 100, 0.005, 150, Mat(), 3,false, 0.04);
    //k- свободный параметр детектора Харриса, рекомендуют устанавливать равным 0,04.
/*
    for (int i = 0; i < src_gray.rows; i++)
    {
        for (int j = 0; j < src_gray.cols; j++)
        {
            if (myShiTomasi_dst.at<float>(i, j) > myShiTomasi_minVal + (myShiTomasi_maxVal - myShiTomasi_minVal) * myShiTomasi_qualityLevel / max_qualityLevel)
            {
                circle(myShiTomasi_copy, Point(j, i), 4, Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256)), FILLED);
            }
        }
    }*/
    // рисуем найденные точки
    cout << "** Number of corners detected: " << corners.size() <<
        endl;
    int radius = 4;
    for (size_t i = 0; i < corners.size(); i++)
    {
        if (corners.size() -i<=4)
        {
            circle(myShiTomasi_copy, corners[i], radius, Scalar(rng.uniform(0, 255),
                rng.uniform(0, 256), rng.uniform(0, 256)), FILLED);
        }
    }
    imshow(myShiTomasi_window, myShiTomasi_copy);
    affine(myShiTomasi_copy);
    
}
void myHarris_function(int, void*)
{
    Mat dst;
    int blockSize = 2;
    int apertureSize = 3;
    double k = 0.04;

    myHarris_copy = src.clone();
    myHarris_qualityLevel = MAX(myHarris_qualityLevel, 1);
    cornerHarris(src_gray, dst, blockSize, apertureSize, k); // параметр апертуры для оператора Собеля
    //свободный параметр детектора Харриса  //метод экстраполяции
    for (int i = 0; i < src_gray.rows; i++)
    {
        for (int j = 0; j < src_gray.cols; j++)
        {
            if (Mc.at<float>(i, j) > myHarris_minVal + (myHarris_maxVal - myHarris_minVal) * myHarris_qualityLevel / max_qualityLevel)
            {
                circle(myHarris_copy, Point(j, i), 4, Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256)), FILLED);
            }
        }
    }
    imshow(myHarris_window, myHarris_copy);
    //affine(myHarris_copy);
}

//афины так себе работают, тольоко для конкретного случая
void affine(Mat src) {
    Point2f srcTri[3];
    srcTri[0] = Point2f(0.f, 0.f);
    srcTri[1] = Point2f(src.cols - 1.f, 0.f);
    srcTri[2] = Point2f(0.f, src.rows - 1.f);
    Point2f dstTri[3];
    dstTri[0] = Point2f(0.f, src.rows * 0.33f);
    dstTri[1] = Point2f(src.cols * 0.85f, src.rows * 0.25f);
    dstTri[2] = Point2f(src.cols * 0.15f, src.rows * 0.7f);
    Mat warp_mat = getAffineTransform(srcTri, dstTri);
    Mat warp_dst = Mat::zeros(src.rows, src.cols, src.type());
    warpAffine(src, warp_dst, warp_mat, warp_dst.size());
    Point center = Point(warp_dst.cols / 2, warp_dst.rows / 2);
    double angle = -6;
    double scale = 0.9;
    Mat rot_mat = getRotationMatrix2D(center, angle, scale);
    Mat warp_rotate_dst;
    warpAffine(warp_dst, warp_rotate_dst, rot_mat, warp_dst.size());
    imshow("Warp + Rotate", warp_rotate_dst);
}



int main(int argc, char** argv)
{
    src = imread("note2.jpg");
    if (src.empty())
    {
        cout << "Could not open or find the image!\n" << endl;
       // cout << "Usage: " << argv[0] << " <Input image>" << endl;
        return -1;
    }
    //resize(src, src, { 500,500 }, 0, 0, INTER_NEAREST);
    cvtColor(src, src_gray, COLOR_BGR2GRAY);
    int blockSize = 3, apertureSize = 3;
    cornerEigenValsAndVecs(src_gray, myHarris_dst, blockSize, apertureSize);
    /* calculate Mc */
    Mc = Mat(src_gray.size(), CV_32FC1);
    for (int i = 0; i < src_gray.rows; i++)
    {
        for (int j = 0; j < src_gray.cols; j++)
        {
            float lambda_1 = myHarris_dst.at<Vec6f>(i, j)[0];
            float lambda_2 = myHarris_dst.at<Vec6f>(i, j)[1];
            Mc.at<float>(i, j) = lambda_1 * lambda_2 - 0.04f * ((lambda_1 + lambda_2) * (lambda_1 + lambda_2));
        }
    }
    minMaxLoc(Mc, &myHarris_minVal, &myHarris_maxVal);
    namedWindow(myHarris_window);
    createTrackbar("Quality Level:", myHarris_window, &myHarris_qualityLevel, max_qualityLevel, myHarris_function);
    myHarris_function(0, 0);
    
    cornerMinEigenVal(src_gray, myShiTomasi_dst, blockSize, apertureSize);
    minMaxLoc(myShiTomasi_dst, &myShiTomasi_minVal, &myShiTomasi_maxVal);
    namedWindow(myShiTomasi_window);
    createTrackbar("Quality Level:", myShiTomasi_window, &myShiTomasi_qualityLevel, max_qualityLevel, myShiTomasi_function);
    myShiTomasi_function(0, 0);

    //affine(src);
    waitKey();
    return 0;
}