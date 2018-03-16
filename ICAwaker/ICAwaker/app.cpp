#include "stdafx.h"

#include "app.hpp"

#include "Window.hpp"
#include "Exception.hpp"
#include "utilities.hpp"
#include "drawing.hpp"

namespace ic
{
	App::App() { }

	App::~App() { }

	int App::main(int argc, char* argv[])
	{
		cv::VideoCapture capture(0);

		Exception::require(capture.isOpened(), CANT_CAPTURE_CAM);

		Window camera(W_CAMERA, Size(640, 480), Point(0, 230));

		cv::CascadeClassifier face_classifier("haarcascade_face.xml");
		cv::CascadeClassifier eye_classifier("haarcascade_eye_tree_eyeglasses.xml");

		while (cv::waitKey(1) != 'q')
		{
			std::vector<Rect> faces;
			Mat frame, frame_gray;

			if (!capture.read(frame))
				throw Exception(CANT_READ_CAM_FRAME);

			cv::cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY);
			cv::equalizeHist(frame_gray, frame_gray);
			face_classifier.detectMultiScale(frame_gray, faces, 1.1, 2, face_options, face_size);

			for (auto face : faces)
			{
				// cv::Exception dla
				//		inline Mat Mat::operator()(const Rect& roi) const
				//		{
				//			return Mat(*this, roi);
				//		}
				//
				// Do refactoru jak zacznie dzialac
				//

				Rect trim(face);

				trim -= Size(face_radius >> 1, height_trimming);
				trim += Point(face_radius >> 2, (height_trimming >> 1) - (height_trimming >> 2));

				Rect left = Rect(trim.x - 5, trim.y, trim.width >> 1, trim.height);
				Rect right = Rect(trim.x + (trim.width >> 1), trim.y, (trim.width >> 1) + 5, trim.height);

				Mat current_left = frame_gray(left);
				Mat current_right = frame_gray(right);

				std::vector<Rect> eyes_left;
				std::vector<Rect> eyes_right;

				eye_classifier.detectMultiScale(current_left, eyes_left, 1.1, 2, eye_options, eye_size);
				eye_classifier.detectMultiScale(current_right, eyes_right, 1.1, 2, eye_options, eye_size);

				eye_detection(frame, face, eyes_left, eyes_right);

				cv::rectangle(frame, trim, cv::Scalar(0, 64, 0), 2);
				cv::rectangle(frame, left, cv::Scalar(0, 0, 128), 1);
				cv::rectangle(frame, right, cv::Scalar(128, 0, 0), 1);
			}

			cv::imshow(camera.name, frame);
		}
	}

	void App::eye_detection(Mat frame, Rect face, std::vector<Rect> left, std::vector<Rect> right)
	{
		for (auto i : left)
			for (auto j : right)
				if (between(i.width, j.width - dispersion, j.width + dispersion))
				{
					draw_eyes(frame, face, i, j);
					return;
				}
	}
}
