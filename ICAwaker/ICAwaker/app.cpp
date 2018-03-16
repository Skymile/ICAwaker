#include "stdafx.h"

#include "Window.hpp"
#include "Exception.hpp"
#include "utilities.hpp"
#include "drawing.hpp"

namespace ic
{
	const int face_size = 96;
	const int eye_size = 32;
	const int dispersion = 32;

	void eye_detection(Mat frame, Rect face, std::vector<Rect> eyes)
	{
		for (auto i : eyes)
			for (auto j : eyes)
				if (i != j && between(i.width, j.width - dispersion, j.width + dispersion))
				{
					draw_eyes(frame, face, i, j);
					return;
				}
	}

	int main(int argc, char* argv[])
	{
		cv::VideoCapture capture(0);

		Exception::require(capture.isOpened(), CANT_CAPTURE_CAM);

		Window camera(W_CAMERA, Size(640, 480), Point(0, 230));

		cv::CascadeClassifier face_classifier("haarcascade_face.xml"),
			eye_classifier("haarcascade_eye_tree_eyeglasses.xml");

		while (cv::waitKey(1) != 'q')
		{
			std::vector<Rect> faces;
			Mat frame, frame_gray;

			if (!capture.read(frame))
				throw Exception(CANT_READ_CAM_FRAME);

			cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
			cv::equalizeHist(frame_gray, frame_gray);
			face_classifier.detectMultiScale(frame_gray, faces, 1.1, 2, cv::CASCADE_SCALE_IMAGE | cv::CASCADE_FIND_BIGGEST_OBJECT, Size(face_size, face_size));

			for (auto face : faces)
			{
				face -= Size(16, face_size >> 1);
				face += Point(8, face_size >> 2);

				Point center = rect_center(face);
				Mat current = frame_gray(face);
				std::vector<Rect> eyes;
				eye_classifier.detectMultiScale(current, eyes, 1.1, 2, cv::CASCADE_SCALE_IMAGE | cv::CASCADE_DO_CANNY_PRUNING, Size(eye_size, eye_size));

				eye_detection(frame, face, eyes);
			}
			cv::imshow(camera.name, frame);
		}
	}
}
