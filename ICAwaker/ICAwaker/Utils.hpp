#pragma once

namespace ic
{
	cv::Point center(cv::Rect a);

	int distance(cv::Rect a, cv::Rect b);

	cv::Rect scale(cv::Rect a, float ratio);

	cv::Rect moveCenter(cv::Rect a, cv::Point center);

	cv::Rect avg(cv::Rect a, cv::Rect b);

	cv::Point centroid(cv::Mat &a);
}
