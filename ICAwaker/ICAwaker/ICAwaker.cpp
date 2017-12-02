// Defines the entry point for the application.

#include "stdafx.h"
#include "EyeDetector.hpp"
#include "Utils.hpp"
#include "Window.hpp"

int main(int argc, char** argv)
{
	cv::VideoCapture videoCapture(0);
	
	if (!videoCapture.isOpened())
	{
		std::cout << "Could not open the camera\n";
		__debugbreak();
		return -1;
	}

	Window leftEyeWindow("Left Eye", cv::Size(256, 256));
	Window rightEyeWindow("Right Eye", cv::Size(256, 256));
	Window faceWindow("Face", cv::Size(200, 200));
	Window cameraWindow("Camera");

//	cv::createTrackbar("Threshold:", "Camera", &threshold, 255);
//	cv::createTrackbar("Scale:", "Camera", &scaleRatio, 100);

	EyeDetector eyeDetector("haarcascade_face.xml", "haarcascade_eye.xml");
	
	try 
	{
		while (cv::waitKey(10) != 'q')
		{
			cv::Mat frame;

			if (!videoCapture.read(frame))
				abort();

			cv::Rect face, leftEye, rightEye;
			eyeDetector.detect(frame, face, leftEye, rightEye, cv::Size(120, 120), cv::Size(250, 250));

			faceWindow.Show(frame(face));
			leftEyeWindow.Show(frame(leftEye), true);
			rightEyeWindow.Show(frame(rightEye), true);

			cv::rectangle(frame, face, cv::Scalar(255, 0, 0));
			cv::rectangle(frame, leftEye, cv::Scalar(0, 255, 0));
			cv::rectangle(frame, rightEye, cv::Scalar(0, 0, 255));
			cameraWindow.Show(frame);
		}
	}
	catch (cv::Exception e)
	{
		std::cout << e.what() << std::endl;
		__debugbreak();
	}

	return 0;
}
