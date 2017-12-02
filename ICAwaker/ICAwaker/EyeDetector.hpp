#pragma once

#include "Utils.hpp"

class EyeDetector
{
public:
	EyeDetector(
		std::string haarCascadeFaceFilename,
		std::string haarCascadeEyeFilename,
		double scaleRatio = 50
	);
	double& scaleRatio();
	void detect(cv::Mat frame, cv::Rect &face, cv::Rect &leftEye, cv::Rect &rightEye, const cv::Size& min_size, const cv::Size& max_size);

private:
	cv::Rect detectFace(const cv::Mat& frame, const cv::Size& minSize, const cv::Size& maxSize);
	std::vector<cv::Rect> _detectFrames(const cv::Mat& frame, const cv::Size& minSize, const cv::Size& maxSize);
	cv::Rect findEyeCenter(cv::Rect eye, const cv::Mat& frame);
	void detectEye(
		const cv::Mat& frame,
		cv::Rect bestFaceObject,
		cv::Rect &leftEye,
		cv::Rect &rightEye,
		const cv::Size& minSize,
		const cv::Size& maxSize
	);

	cv::CascadeClassifier _face_cascade, _eye_cascade;
	cv::Rect _lastFaceObject;
	cv::Rect _lastLeftEyeObject;
	cv::Rect _lastRightEyeObject;
	double _scaleRatio;
};
