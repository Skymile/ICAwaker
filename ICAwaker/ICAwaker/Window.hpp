#pragma once

#include "stdafx.h"

#define W_CAMERA "Camera Window"
//#define W_LEFT_EYE "Left Eye Window"
//#define W_RIGHT_EYE "Right Eye Window"
//#define W_FACE "Face Window"

namespace ic
{
	struct Window
	{
		Window(const char* name, const cv::Size & size, const cv::Point & position = cv::Point(0, 0), int flags = 0);

		const char* name;
	};
}
