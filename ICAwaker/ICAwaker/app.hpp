#pragma once

namespace ic
{
	class App
	{
	public:
		App();
		~App();

		int main(int argc, char* argv[]);

	private:
		void App::eye_detection(Mat frame, Rect face, std::vector<Rect> left, std::vector<Rect> right);

		const int face_radius = 96;
		const int eye_radius = 32;

		const Size eye_size = Size(eye_radius, eye_radius);
		const Size face_size = Size(face_radius, face_radius);

		const int dispersion = 16;
		const int height_trimming = 128;

		const int face_options = cv::CASCADE_SCALE_IMAGE | cv::CASCADE_FIND_BIGGEST_OBJECT;
		const int eye_options = cv::CASCADE_SCALE_IMAGE | cv::CASCADE_DO_CANNY_PRUNING;
	};
}