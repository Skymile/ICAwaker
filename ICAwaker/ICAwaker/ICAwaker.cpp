// Defines the entry point for the application.

#include "stdafx.h"

int main(int argc, char** argv)
{
	cv::VideoCapture videoCapture(1);
	if (!videoCapture.isOpened()) abort();

	cv::namedWindow("cam", cv::WINDOW_AUTOSIZE);

	for (;;) {
		cv::Mat frame;
		if (!videoCapture.read(frame)) abort();
		cv::imshow("cam", frame);
		cv::waitKey(30);

	}

	return 0;
}
