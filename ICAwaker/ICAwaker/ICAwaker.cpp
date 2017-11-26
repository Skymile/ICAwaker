// Defines the entry point for the application.

#include "stdafx.h"

int distance(cv::Rect a, cv::Rect b) 
{
	auto aCenter = (a.br() + a.tl()) * 0.5;
	auto bCenter = (b.br() + b.tl()) * 0.5;
	return norm(aCenter - bCenter);	
}

int main(int argc, char** argv)
{
	cv::VideoCapture videoCapture(1);
	if (!videoCapture.isOpened()) abort();

	cv::CascadeClassifier face_cascade, eye_cascade;
	if (!face_cascade.load("haarcascade_face.xml")) abort();
	if (!eye_cascade.load("haarcascade_eye.xml")) abort();

	cv::namedWindow("face", 0);
	cv::resizeWindow("face", cv::Size(256, 256));
	cv::namedWindow("cam");
	cv::namedWindow("eye", 0);
	cv::resizeWindow("eye", cv::Size(128, 128));

	cv::Rect lastObject;
	for (;;) {
		cv::Mat frame;
		if (!videoCapture.read(frame)) abort();
		cv::Mat frameGray;
		cv::cvtColor(frame, frameGray, CV_BGR2GRAY);
		cv::equalizeHist(frameGray, frameGray);

		std::vector<cv::Rect> faceObjects;
		face_cascade.detectMultiScale(frameGray, faceObjects, 1.03, 2, 0, cv::Size(80, 80), cv::Size(150, 150));

		cv::Rect bestObject;
		int minDinstacne = INT_MAX;
		for (auto o : faceObjects) {
			auto dist = distance(lastObject, o);
			if (dist < minDinstacne) {
				bestObject = o;
				minDinstacne = dist;
			}
		}

		if (bestObject.area() == 0) bestObject = lastObject;
		else lastObject = bestObject;

		if (bestObject.area() != 0) {
			auto frameFace = frame(bestObject);
			std::vector<cv::Rect> eyeObjects;
			eye_cascade.detectMultiScale(frameFace, eyeObjects, 1.02, 2, 0, cv::Size(30, 30), cv::Size(60, 60));

			for (auto o : eyeObjects) {
				cv::rectangle(frameFace, o, cv::Scalar(0, 255, 0));
			}

			if(eyeObjects.size() > 0) cv::imshow("eye", frameFace(eyeObjects[0]));

			cv::imshow("face", frameFace);
		}

		for (auto o : faceObjects) {
			cv::rectangle(frame, o, cv::Scalar(255, 0, 0));
		}

		cv::imshow("cam", frame);

		if (cv::waitKey(30) == 'q') break;
	}

	return 0;
}
