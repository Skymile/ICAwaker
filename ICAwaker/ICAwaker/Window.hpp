#pragma once

#include "stdafx.h"

class Window
{
public:
	Window(std::string windowName);
	Window(std::string windowName, cv::Size size);
	~Window();

	void Show(cv::Mat frame, bool grayOut = false);
	std::string GetName();

private:
	std::string name;
};
