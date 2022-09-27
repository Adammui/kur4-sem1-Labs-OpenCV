// Lab2-OpenCV.cpp : Reynes lab
//
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include<math.h>  
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;
using namespace std;

int DELAY_CAPTION = 1500;
int DELAY_BLUR = 100;
int MAX_KERNEL_LENGTH = 31;

void task1(Mat image);
void task2(Mat image);
void task3_4(Mat image);
void display_image(Mat img, String str);
// void build_hist(String window, String hist, Mat src);
// Mat equalize_image(Mat image);


void task1(Mat image) {

    Mat dst;
    const float kernelData[] = { -1.0f, -1.0f, -1.0f, -1.0f, 9.0f, -1.0f, -1.0f, -1.0f, -1.0f };
    const Mat kernel(3, 3, CV_32FC1, (float*)kernelData);
    filter2D(image, dst, -1, kernel);
    display_image(dst, "image_1");
    imwrite("img_out.jpg", dst);
    printf("Image_1 saved \n");
}

void task2(Mat image) {
    Mat dst, dst2, dst3;
    for (int i = 1; i < MAX_KERNEL_LENGTH; i = i + 2)
    {
        blur(image, dst, Size(i, i), Point(-1, -1));
        display_image(dst, "image_blur");

        medianBlur(image, dst2, i);
        display_image(dst2, "image_medianblur");

        GaussianBlur(image, dst3, Size(i, i), 0, 0);
        display_image(dst3, "image_gaussianblur");
    }
}

void task3_4(Mat image) {
    Mat img_gray, img_bw, erodeImg, dilateImg;
    cvtColor(image, img_gray, COLOR_BGR2GRAY);
    threshold(img_gray, img_bw, 200, 255, THRESH_BINARY);

    Mat element = Mat(); // ядро по умолчанию
    erode(img_bw.clone(), erodeImg, element); // вычисление эрозии(сужение)
    display_image(erodeImg, "erodeImg");
    dilate(img_bw.clone(), dilateImg, element);
    display_image(dilateImg, "dilateImg");

    Mat diff_im = img_bw - erodeImg;
    display_image(diff_im, "diff_Img");
}

void display_image(Mat img, String str) {
    namedWindow(str, WINDOW_AUTOSIZE);
    imshow(str, img);
}

int main()
{
    Mat image;
    image = imread("img_test.jpg", 1);
    resize(image, image, { 500,500 }, 0, 0, INTER_NEAREST);
    if (!image.data)
    {
        printf("No image data \n");
        return -1;
    }
    display_image(image,"image");
    //task1(image);
    //task2(image);
    task3_4(image);

    waitKey(0);
    return 0;
}