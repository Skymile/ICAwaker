// Defines the entry point for the application.

#include "stdafx.h"
#include "EyeDetector.h"
#include "Utils.h"

int main(int argc, char** argv)
{
	try {
		cv::VideoCapture videoCapture(1);
		if (!videoCapture.isOpened()) abort();

		auto leftEyeWindowName = "left eye";
		auto rightEyeWindowName = "right eye";
		auto faceWindowName = "face";
		auto camWindowName = "cam";

		cv::namedWindow(faceWindowName, 0);
		cv::resizeWindow(faceWindowName, cv::Size(256, 256));
		cv::namedWindow(camWindowName);
		cv::namedWindow(leftEyeWindowName, 0);
		cv::resizeWindow(leftEyeWindowName, cv::Size(200, 200));
		cv::namedWindow(rightEyeWindowName, 0);
		cv::resizeWindow(rightEyeWindowName, cv::Size(200, 200));

		//cv::createTrackbar("Threshold:", camWindowName, &threshold, 255);
		//cv::createTrackbar("Scale:", camWindowName, &scaleRatio, 100);

		EyeDetector eyeDetector("haarcascade_face.xml", "haarcascade_eye.xml");

		for (;;) {
			cv::Mat frame;
			if (!videoCapture.read(frame)) abort();
			cv::Rect face, leftEye, rightEye;
			eyeDetector.detect(frame, face, leftEye, rightEye);

			auto faceFrame = frame(face);
			auto leftEyeFrame = frame(leftEye);
			auto rightEyeFrame = frame(rightEye);
			cv::cvtColor(leftEyeFrame, leftEyeFrame, CV_BGR2GRAY);
			cv::equalizeHist(leftEyeFrame, leftEyeFrame);
			cv::cvtColor(rightEyeFrame, rightEyeFrame, CV_BGR2GRAY);
			cv::equalizeHist(rightEyeFrame, rightEyeFrame);

			cv::imshow(faceWindowName, faceFrame);
			cv::imshow(leftEyeWindowName, leftEyeFrame);
			cv::imshow(rightEyeWindowName, rightEyeFrame);

			cv::rectangle(frame, face, cv::Scalar(255, 0, 0));
			cv::rectangle(frame, leftEye, cv::Scalar(0, 255, 0));
			cv::rectangle(frame, rightEye, cv::Scalar(0, 0, 255));
			cv::imshow(camWindowName, frame);

			if (cv::waitKey(1) == 'q') break;
		}
	}
	catch (cv::Exception e)
	{
		std::cout << e.what() << std::endl;
		__debugbreak();
	}

	return 0;
}
