// Defines the entry point for the application.

#include "stdafx.h"
#include "EyeDetector.h"
#include "Utils.h"
#include "EyeBlinkDetector.h"

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
		cv::resizeWindow(faceWindowName, cv::Size(200, 200));
		cv::moveWindow(faceWindowName, 402, 0);

		cv::namedWindow(camWindowName);
		cv::moveWindow(camWindowName, 0, 230);

		cv::namedWindow(leftEyeWindowName, 0);
		cv::resizeWindow(leftEyeWindowName, cv::Size(200, 200));
		cv::moveWindow(leftEyeWindowName, 0, 0);

		cv::namedWindow(rightEyeWindowName, 0);
		cv::resizeWindow(rightEyeWindowName, cv::Size(200, 200));
		cv::moveWindow(rightEyeWindowName, 201, 0);

		cv::namedWindow("debug", 0);
		cv::resizeWindow("debug", 200, 200);
		cv::moveWindow("debug", 603, 0);

		//cv::createTrackbar("Threshold:", camWindowName, &threshold, 255);

		EyeDetector eyeDetector("haarcascade_face.xml", "haarcascade_eye.xml");
		EyeBlinkDetector blinkDetector;

		int threshold = 55;
		cv::createTrackbar("Scale:", camWindowName, &threshold, 100);

		for (;;) {
			eyeDetector.scaleRatio() = threshold;

			cv::Mat frame;
			if (!videoCapture.read(frame)) abort();

			cv::Rect face, leftEye, rightEye;
			bool faceDetected, leftEyeDetected, rightEyeDetected;
			auto detected = eyeDetector.detect(frame, face, leftEye, rightEye,
				faceDetected, leftEyeDetected, rightEyeDetected);

			if (faceDetected) {
				auto faceFrame = frame(face);
				cv::imshow(faceWindowName, faceFrame);
				if (detected) cv::rectangle(frame, face, cv::Scalar(255, 0, 0));
				else cv::rectangle(frame, face, cv::Scalar(0, 0, 255));
			}

			if (leftEyeDetected) {
				blinkDetector.detect(frame, leftEye);

				auto leftEyeFrame = frame(leftEye);
				cv::cvtColor(leftEyeFrame, leftEyeFrame, CV_BGR2GRAY);
				cv::equalizeHist(leftEyeFrame, leftEyeFrame);
				cv::imshow(leftEyeWindowName, leftEyeFrame);
				cv::rectangle(frame, leftEye, cv::Scalar(0, 255, 0));
			}

			if (rightEyeDetected) {
				auto rightEyeFrame = frame(rightEye);
				cv::cvtColor(rightEyeFrame, rightEyeFrame, CV_BGR2GRAY);
				cv::equalizeHist(rightEyeFrame, rightEyeFrame);
				cv::imshow(rightEyeWindowName, rightEyeFrame);
				cv::rectangle(frame, rightEye, cv::Scalar(0, 0, 255));
			}			
			
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
