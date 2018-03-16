#include "stdafx.h"
#include "drawing.hpp"

void draw_circle(Mat frame, Rect face, Rect eye)
{
	cv::circle(
		frame,
		Point(face.x + eye.x + eye.width / 2, face.y + eye.y + eye.height / 2),
		cvRound(eye.width + eye.height) / 4,
		cv::Scalar(0, 0, 255),
		2
	);
}

void draw_eyes(Mat frame, Rect face, Rect eye_one, Rect eye_two)
{
	draw_circle(frame, face, eye_one);
	draw_circle(frame, face, eye_two);
}
