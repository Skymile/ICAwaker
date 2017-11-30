#pragma once

cv::Point center(cv::Rect a)
{
	return  (a.br() + a.tl()) * 0.5;;
}

int distance(cv::Rect a, cv::Rect b)
{
	return norm(center(a) - center(b));
}


cv::Rect scale(cv::Rect a, float ratio)
{
	a.x += a.width * (1 - ratio) / 2;
	a.y += a.height * (1 - ratio) / 2;
	a.width *= ratio;
	a.height *= ratio;

	return a;
}


cv::Rect moveCenter(cv::Rect a, cv::Point center)
{
	auto aCenter = (a.br() + a.tl()) * 0.5;
	auto distanceX = center.x - aCenter.x;
	auto distanceY = center.y - aCenter.y;
	a.x += distanceX;
	a.y += distanceY;
	return a;
}

cv::Rect avg(cv::Rect a, cv::Rect b)
{
	return cv::Rect((a.x + b.x) / 2, (a.y + b.y) / 2,
		(a.width + b.width) / 2, (a.height + b.height) / 2);
}

cv::Point centroid(cv::Mat &a)
{
	auto m = cv::moments(a, false);
	cv::Point p(m.m10 / m.m00, m.m01 / m.m00);
	return p;
}