#pragma once

#define _CRT_SECURE_NO_WARNINGS 

#include "targetver.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm>
#include <memory>

#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/features2d.hpp>

//#include <dlib/image_processing/frontal_face_detector.h>
//#include <dlib/image_processing/render_face_detections.h>
//#include <dlib/image_processing.h>
//#include <dlib/gui_widgets.h>
//#include <dlib/image_io.h>

using byte = unsigned char;

using cv::Point;
using cv::Size;
using cv::Mat;
using cv::Rect;
