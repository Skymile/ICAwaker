#include "stdafx.h"
#include "Window.hpp"

namespace ic
{
	Window::Window(const char* name, const cv::Size & size, const cv::Point & position, int flags)
		: name(name)
	{
		cv::namedWindow(name, flags);
		cv::resizeWindow(name, size);
		cv::moveWindow(name, position.x, position.y);
	}
}