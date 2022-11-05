#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include<math.h>  
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <stdlib.h>
#include <stdio.h>
#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/objdetect/objdetect.hpp>
#include <E:\tools\4u1hij\multym\opencv_contrib-4.x\modules\tracking\include\opencv2/tracking.hpp>
//#include <opencv2/core/ocl.hpp>
//#include <opencv2/face.hpp>
#include <opencv2/video/background_segm.hpp>

#define DELAY 30
#define ESC_KEY 27
using namespace cv;
using namespace std;
//using namespace cv::face;
Mat frame;
String face_cascade_name = "haarcascade_frontalface_default.xml";
String smile_cascade_name = "haarcascade_smile.xml";
CascadeClassifier face_cascade;
CascadeClassifier smile_cascade;

void lucasKanade()
{
	//когерентность
	string filename = "a.gif";

	VideoCapture capture(0);
	if (!capture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open file!" << endl;
	}
	// Create some random colors малые изм and яркость
	vector<Scalar> colors;
	RNG rng;
	for (int i = 0; i < 100; i++)
	{
		int r = rng.uniform(0, 256);
		int g = rng.uniform(0, 256);
		int b = rng.uniform(0, 256);
		colors.push_back(Scalar(r, g, b));
	}
	Mat old_frame, old_gray;
	vector<Point2f> p0, p1;
	// Take first frame and find corners in it
	capture >> old_frame;
	cvtColor(old_frame, old_gray, COLOR_BGR2GRAY);
	goodFeaturesToTrack(old_gray, p0, 100, 0.3, 7, Mat(), 7, false, 0.04);
	// Create a mask image for drawing purposes
	Mat mask = Mat::zeros(old_frame.size(), old_frame.type());
	while (true) {
		Mat frame, frame_gray;
		capture >> frame;
		if (frame.empty())
			break;
		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		// calculate optical flow
		vector<uchar> status;
		vector<float> err;
		//определяет критерии завершения для итерационных алгоритмов
		TermCriteria criteria = TermCriteria((TermCriteria::COUNT)+(TermCriteria::EPS), 10, 0.03);
		
		calcOpticalFlowPyrLK(old_gray, frame_gray, p0, p1, status, err, Size(15, 15), 2, criteria);
		//
		vector<Point2f> good_new;
		for (uint i = 0; i < p0.size(); i++)
		{
			// Select good points
			if (status[i] == 1) {
				good_new.push_back(p1[i]);
				// draw the tracks
				line(mask, p1[i], p0[i], colors[i], 2);
				circle(frame, p1[i], 5, colors[i], -1);
			}
		}
		Mat img;
		add(frame, mask, img);
		imshow("Frame", img);
		// Exit if ESC pressed.
		int k = waitKey(1);
		if (k == 27)
		{
			break;
		}
		// Now update the previous frame and previous points
		old_gray = frame_gray.clone();
		p0 = good_new;
	}

}

void backSubstractor()
{

	//create Background Subtractor objects
	Ptr<BackgroundSubtractor> pBackSub;
	//pBackSub = createBackgroundSubtractorMOG2();
	pBackSub = createBackgroundSubtractorKNN();
	VideoCapture capture(0);
	if (!capture.isOpened()) {
		//error in opening the video input
		cerr << "Unable to open: " << endl;
	}
	Mat frame, fgMask;
	while (true) {
		capture >> frame;
		if (frame.empty())
			break;
		//update the background model
		pBackSub->apply(frame, fgMask);
		//get the frame number and write it on the current frame
		rectangle(frame, cv::Point(10, 2), cv::Point(100, 20),
			cv::Scalar(255, 255, 255), -1);
		stringstream ss;
		ss << capture.get(CAP_PROP_POS_FRAMES);
		string frameNumberString = ss.str();
		putText(frame, frameNumberString.c_str(), cv::Point(15, 15),
			FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
		//show the current frame and the fg masks
		imshow("Frame", frame);
		imshow("FG Mask", fgMask);
		// Exit if ESC pressed.
		int k = waitKey(1);
		if (k == 27)
		{
			break;
		}
	}
}
void objectTracker()
{
	// List of tracker types in OpenCV 3.4.1
	string trackerTypes[7] = { "BOOSTING", "MIL", "KCF", "TLD","MEDIANFLOW", "GOTURN", "MOSSE" };
	//MIL генерирует классификатор в онлайн - режиме, чтобы отделить
		//объект от фона;
	// Create a tracker
	string trackerType = trackerTypes[1];//
	Ptr<Tracker> tracker = TrackerMIL::create();
	// Read video
	VideoCapture video("a.gif");
	// Exit if video is not opened
	if (!video.isOpened())
	{
		cout << "Could not read video file" << endl;
	}
	// Read first frame 
	Mat frame;
	bool ok = video.read(frame);
	Rect bbox(100, 100, 150, 150);
	rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
	imshow("Tracking", frame);
	tracker->init(frame, bbox);

	while (video.read(frame))
	{
		// Update the tracking result
		bool ok = tracker->update(frame, bbox);
		if (ok)
		{
			rectangle(frame, bbox, Scalar(255, 0, 0), 2, 1);
		}
		else
		{
			// Tracking failure detected.
			putText(frame, "Tracking failure detected", Point(100, 80),
				FONT_HERSHEY_SIMPLEX, 0.75, Scalar(0, 0, 255), 2);
		}
		// Display tracker type on frame
		putText(frame, trackerType + " Tracker", Point(100, 20),
			FONT_HERSHEY_SIMPLEX, 0.75, Scalar(50, 170, 50), 2);
		// Display frame.
		imshow("Tracking", frame);

		// Exit if ESC pressed.
		int k = waitKey(1);
		if (k == 27)
		{
			break;
		}
	}
}
int main()
{
	//backSubstractor();
	//lucasKanade();
	objectTracker();

	waitKey();
	return 0;
}
