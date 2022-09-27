// Lab1-OpenCV.cpp REYNE'S lab

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include<math.h>   

using namespace cv;
using namespace std;


void save_image(string path, Mat img)
{
	imwrite(path, img);
}
void task12() 
{
	//task 1
	string img_path = "img_test.jpg";
	Mat image = imread(img_path, IMREAD_COLOR);
	resize(image, image, { 500,500 }, 0, 0, INTER_NEAREST);
	imshow("My Background", image);

	Mat image_gray = imread(img_path, IMREAD_GRAYSCALE);
	resize(image_gray, image_gray, { 500,500 }, 0, 0, INTER_NEAREST);
	imshow("My Background-gray", image_gray);

	Mat image_bw;
	threshold(image_gray, image_bw, 150, 255, THRESH_BINARY);

	//Mat image_bw = image_gray > 128; 
	//работает так же как строчка выше, по идее режет кол-во каналов так что остаются только черные и белые цвета
	resize(image_bw, image_bw, { 500,500 }, 0, 0, INTER_NEAREST);
	imshow("My Background-black and white", image_bw);

	//task 2
	save_image("img_bw.jpg", image_bw);
}

void draw_hist(Mat img_start, string number) {

	imshow("Original"+number, img_start);

	// Отображение по каналу
	vector<Mat>bgr_planes;
	split(img_start, bgr_planes);
	// Делим многоканальное изображение на несколько одноканальных изображений const Mat

	// вычисляем гистограмму
	int histsize = 256;
	float range[] = { 0,256 };
	const float* hist_ranges = { range };
	Mat b_hist, g_hist, r_hist;
	calcHist(&bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histsize, &hist_ranges, true, false);
	calcHist(&bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histsize, &hist_ranges, true, false);
	calcHist(&bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histsize, &hist_ranges, true, false);

	// нормализуем
	int hist_h = 400; // Высота изображения гистограммы
	int hist_w = 512; //// Ширина изображения гистограммы
	int bin_w = hist_w / histsize; // Уровень гистограммы
	Mat hist_image(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0)); // Рисуем изображение, отображаемое гистограммой
	normalize(b_hist, b_hist, 0, hist_h, NORM_MINMAX, -1, Mat()); // нормализация
	normalize(g_hist, g_hist, 0, hist_h, NORM_MINMAX, -1, Mat());

	for (int i = 1; i < histsize; i++)
	{
		// Рисуем синюю компонентную гистограмму
		line(hist_image, Point((i - 1) * bin_w, hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2);
		// Рисуем зеленую компонентную гистограмму
		line(hist_image, Point((i - 1) * bin_w, hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2);
		// Рисуем красную компонентную гистограмму
		line(hist_image, Point((i - 1) * bin_w, hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point((i)*bin_w, hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2);
	}
	imshow("Гистограмма"+number, hist_image);
}
void round_osv(Mat old_img, string number) {
	Mat image_gray, image_equal;

	/* этот код по идее для чернобелых фоток пойдет, не делит на 3 потока

	// Преобразование в изображение в градациях серого, чтобы сделать исходное изображение одноканальным
	cvtColor(old_img, image_gray, COLOR_BGR2GRAY);    ////
	// Просмотр количества каналов изображения
	int c = image_gray.channels();
	cout << "=" << endl << c << endl;
	// Выравнивание гистограммы
	equalizeHist(image_gray, image_equal);
	*/

	vector<Mat>channels;
	split(old_img, channels);
	Mat blue, green, red;
	blue = channels.at(0);
	green = channels.at(1);
	red = channels.at(2);

	equalizeHist(blue, blue);
	equalizeHist(green, green);
	equalizeHist(red, red);
	
	merge(channels, image_equal);

	draw_hist(image_equal, number+"1");
}
void task3(string img_path)
{
	Mat img;
	img = imread(img_path);
	draw_hist(img, "1");
	round_osv(img, "2");
}
int main()
{
	string img_zasvet_path = "img_zasvet.jpg";
	string img_zatem_path = "img_zatem.jpg";

	//task12();
	task3(img_zasvet_path);
	//task3(img_zatem_path);

	waitKey(0);
	return 0;
}
