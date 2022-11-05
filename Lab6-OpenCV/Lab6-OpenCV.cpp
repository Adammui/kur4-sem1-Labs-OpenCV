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
#include "drawLandmarks.hpp"

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

/*void faceWithFeatures()
{
	// Load Face Detector
	CascadeClassifier faceDetector("haarcascade_frontalface_alt2.xml");
	// Create an instance of Facemark
	Ptr<Facemark> facemark = FacemarkLBF::create();
	// Load landmark detector
	facemark->loadModel("lbfmodel.yaml");
	// Set up webcam for video capture
	VideoCapture cam(0);
	// Variable to store a video frame and its grayscale 
	Mat frame, gray;
	// Read a frame
	while (cam.read(frame))
	{
		// Find face
		vector<Rect> faces;
		// Convert frame to grayscale because
		// faceDetector requires grayscale image.
		cvtColor(frame, gray, COLOR_BGR2GRAY);
		// Detect faces
		faceDetector.detectMultiScale(gray, faces);
		// Variable for landmarks. 
		 // Landmarks for one face is a vector of points
		 // There can be more than one face in the image. Hence, we 
		 // use a vector of vector of points. 
		vector< vector<Point2f> > landmarks;
		// Run landmark detector
		bool success = facemark->fit(frame, faces, landmarks);
		if (success)
		{
			// If successful, render the landmarks on the face
			for (size_t i = 0; i < faces.size(); i++)
			{
				cv::rectangle(frame, faces[i], Scalar(0, 255, 0), 3);
			}
			for (int i = 0; i < landmarks.size(); i++)
			{
				drawLandmarks(frame, landmarks[i]);
				//for (size_t j = 0; j < landmarks[i].size(); j++)
				 //circle(frame, Point(landmarks[i][j].x, landmarks[i][j].y), 1, Scalar(255, 0, 0), 2);
			}
		}
		// Display results 
		imshow("Facial Landmark Detection", frame);
		// Exit loop if ESC is pressed
		if (waitKey(1) == 27) break;
	}
}
*/

void smileDetect()
{
	// создание объект классификатора

	// открытие видео с камеры
	VideoCapture capture(0);
	if (!capture.isOpened()) {
		cerr << "Unable to open: " << endl;
	}
	if (!face_cascade.load(face_cascade_name))
	{
		printf("--(!)Error loading face cascade\n");
	};
	if (!smile_cascade.load(smile_cascade_name))
	{
		printf("--(!)Error loading eyes cascade\n");
	};

	while (true)
	{
		capture >> frame;

		std::vector<Rect> faces;
		Mat frame_gray;

		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);
		face_cascade.detectMultiScale(frame_gray, faces, 1.05, 8, CASCADE_SCALE_IMAGE);

		for (size_t i = 0; i < faces.size(); i++)
		{
			rectangle(frame, faces[i], Scalar(255, 0, 0), 2, 8, 0);

			Mat faceROI = frame_gray(faces[i]);
			std::vector<Rect> smile;

			//-- In each face, detect smile
			smile_cascade.detectMultiScale(faceROI, smile, 1.1, 55, CASCADE_SCALE_IMAGE);

			for (size_t j = 0; j < smile.size(); j++)
			{

				Rect rect(faces[i].x + smile[j].x, faces[i].y + smile[j].y, smile[j].width, smile[j].height);
				rectangle(frame, rect, Scalar(0, 0, 255), 2, 8, 0);
				Point text_position(faces[i].x + smile[j].x-20, faces[i].y + smile[j].y-20); 
				putText(frame, "smile", text_position, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255), 1);
			}
		}
		//-- Show what you got  
		imshow("smile", frame);
		int keyboard = waitKey(30);
		if (keyboard == 'q' || keyboard == 27)
		break;
	}
	
}

void faceDetectionWebCamera()
{
	// создание объект классификатора
	CascadeClassifier faceCascade;
	string faceCascadeName = "haarcascade_frontalface_alt.xml";
	faceCascade.load(faceCascadeName);
	// открытие видео с камеры
	VideoCapture capture(0);
	if (!capture.isOpened()) {
		cerr << "Unable to open: " << endl;
	}
	while (true)
	{
		capture >> frame;
		// подготовка изображения
		Mat grayFrame;
		cvtColor(frame, grayFrame, COLOR_BGR2GRAY);
		equalizeHist(grayFrame, grayFrame);
		// обнаружение объектов
		vector<Rect> faces;
		faceCascade.detectMultiScale(grayFrame, faces, 1.1, 2, 0 |
			CASCADE_SCALE_IMAGE, Size(30, 30));
		// перебор найденных объектов и отрисовка прямоугольников 
		for (int i = 0; i < faces.size(); i++)
		{
			Point center(faces[i].x + faces[i].width / 2,
				faces[i].y + faces[i].height / 2);
			Mat faceROI = grayFrame(faces[i]);
			for (i = 0; i < faces.size(); i++)
			{
				rectangle(frame,
					Point(faces[i].x, faces[i].y),
					Point(faces[i].x + faces[i].width,
						faces[i].y + faces[i].height),
					CV_RGB(255, 0, 0), 2);
			}
		}
		imshow("video", frame);
		int keyboard = waitKey(30);
		if (keyboard == 'q' || keyboard == 27)
			break;
	}
}

int main()
{
	//faceDetectionWebCamera();
	//smileDetect();

	waitKey();
	return 0;
}
