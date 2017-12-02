#pragma once

#include "stdafx.h"
#include "Window.hpp"

Window::Window(std::string windowName)
{
	cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE);
	this->name = windowName;
}

Window::Window(std::string windowName, cv::Size size)
{
	cv::namedWindow(windowName, CV_WINDOW_NORMAL);
	cv::resizeWindow(windowName, size);
	this->name = windowName;
}

void Window::Show(cv::Mat frame, bool grayOut)
{
	if (grayOut && frame.channels() > 1)
	{
		cvtColor(frame, frame, CV_BGR2GRAY);
		cv::equalizeHist(frame, frame);
	}

	if (!frame.empty())
		cv::imshow(this->name, frame);
}

std::string Window::GetName()
{
	return this->name;
}

Window::~Window()
{
}
