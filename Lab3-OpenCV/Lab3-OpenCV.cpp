// Lab3-OpenCV.cpp : Этот файл содержит функцию "main". Здесь начинается лаба 3.
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

Mat Canny(Mat img)
{
    Mat gray_img, edges_img;
    double low_threshold = 20, upp_threshold = 100;
    // удаление шумов 
    GaussianBlur(img, img, Size(3, 3),
        0, 0, BORDER_DEFAULT);
    // преобразование в оттенки серого 
    cvtColor(img, gray_img, COLOR_BGR2GRAY);
    // применение детектора Канни 
    Canny(gray_img, edges_img, low_threshold, upp_threshold);
    return edges_img;
}

Mat LaplacianOperator(Mat img)
{
    Mat gray_img, temp_laplacian, laplacian_img;
    int ddepth = CV_16S;
    // сглаживание с помощью фильтра Гаусса 
    GaussianBlur(img, img, Size(3, 3),
        0, 0, BORDER_DEFAULT);
    // преобразование в оттенки серого 
    cvtColor(img, gray_img, COLOR_BGR2GRAY);
    // применение оператора Лапласа 
    Laplacian(gray_img, temp_laplacian, ddepth);
    convertScaleAbs(temp_laplacian, laplacian_img);
    return laplacian_img;
}

Mat SobelOperator(Mat img)
{
    Mat sobel_img, x_grad, y_grad, x_grad_abs, y_grad_abs;
    int ddepth = CV_16S;
    double alpha = 0.5, beta = 0.5;
   //----
    // сглаживание исходного изображения img с помощью фильтра Гаусса
    GaussianBlur(img, img, Size(3, 3), 0, 0, BORDER_DEFAULT);
    // преобразование сглаженного изображения в оттенки серого
    cvtColor(img, img, COLOR_BGR2GRAY);
    // вычисление производных по двум направлениям
    Sobel(img,x_grad, ddepth, 1, 0); // по Ox
    Sobel(img,y_grad, ddepth, 0, 1); // по Oy
    // преобразование градиентов в 8-битные
    convertScaleAbs(x_grad, x_grad_abs);
    convertScaleAbs(y_grad, y_grad_abs);
    // поэлементное вычисление взвешенной суммы двух массивов
    addWeighted(x_grad_abs, alpha, y_grad_abs, beta, 0, sobel_img);
    return sobel_img;
}

int main()
{
    namedWindow("normal video", WINDOW_AUTOSIZE);
    namedWindow("Sobel", WINDOW_AUTOSIZE);
    namedWindow("Laplacian", WINDOW_AUTOSIZE);
    namedWindow("unCanny", WINDOW_AUTOSIZE);
    while (true) {
        // видео с камеры
        //VideoCapture capture(0);
        
        VideoCapture capture("coffe.mp4");

        // проверка корректности открытия 
        if (!capture.isOpened()) {
            cerr << "Unable to open: " << endl;
            return 0;
        }
        Mat frame;
        while (true) {
            // получение кадра видеопотока
            bool suc = capture.read(frame);
            // вывод кадра, если он есть
            if (suc == false)
            {  break; }
            resize(frame, frame, { 500,300 }, 0, 0, INTER_NEAREST);
//--------
            imshow("normal video",frame);
            //заданиe 1 - собел
            imshow("Sobel", SobelOperator(frame));
            //заданиe 2 - laplacian
            imshow("Laplacian", LaplacianOperator(frame));
            //заданиe 3 - canny
            imshow("unCanny", Canny(frame));
//--------
            // условие для выхода из цикла 
            int keyboard = waitKey(30);
            if (keyboard == 'q' || keyboard == 27)
                break;
        }
    }
    return 0;
}