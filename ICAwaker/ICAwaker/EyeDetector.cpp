#pragma once

#include "stdafx.h"
#include "EyeDetector.hpp"

EyeDetector::EyeDetector(
	std::string haarCascadeFaceFilename,
	std::string haarCascadeEyeFilename,
	double scaleRatio
) :
	_face_cascade(haarCascadeFaceFilename),
	_eye_cascade(haarCascadeEyeFilename),
	_scaleRatio(scaleRatio),
	_lastLeftEyeObject(0, 0, 1, 1),
	_lastRightEyeObject(0, 0, 1, 1)
{
}

double& EyeDetector::scaleRatio()
{
	return _scaleRatio;
}

void EyeDetector::detect(cv::Mat frame, cv::Rect &face, cv::Rect &leftEye, cv::Rect &rightEye, const cv::Size& minSize, const cv::Size& maxSize)
{
	if (auto bestFaceObject = detectFace(frame, minSize, maxSize); bestFaceObject.area() != 0) {
		face = bestFaceObject;
		cv::Rect bestLeftEyeObject1, bestRightEyeObject2;
		detectEye(frame, bestFaceObject, bestLeftEyeObject1, bestRightEyeObject2, cv::Size(60, 60), cv::Size(90, 90));
		leftEye = bestLeftEyeObject1 + bestFaceObject.tl();
		rightEye = bestRightEyeObject2 + bestFaceObject.tl();
	}
}

cv::Rect EyeDetector::detectFace(const cv::Mat& frame, const cv::Size& minSize, const cv::Size& maxSize)
	{
		cv::Rect bestFaceObject;
		int minDistance = INT_MAX;
		for (auto o : _detectFrames(frame, minSize, maxSize)) {
			if (int dist = ic::distance(_lastFaceObject, o); dist < minDistance) {
				bestFaceObject = o;
				minDistance = dist;
			}
		}

		if (bestFaceObject.area() == 0)
			bestFaceObject = _lastFaceObject;
		else
			_lastFaceObject = bestFaceObject;

		return bestFaceObject;
	}

std::vector<cv::Rect> EyeDetector::_detectFrames(const cv::Mat& frame, const cv::Size& minSize, const cv::Size& maxSize)
{
	cv::Mat frameGray;
	cv::cvtColor(frame, frameGray, CV_BGR2GRAY);
	cv::equalizeHist(frameGray, frameGray);

	std::vector<cv::Rect> faceObjects;
	_face_cascade.detectMultiScale(frameGray, faceObjects, 1.03, 2, 0, minSize, maxSize);

	return faceObjects;
}

cv::Rect EyeDetector::findEyeCenter(cv::Rect eye, const cv::Mat& frame)
{
	auto roi = ic::scale(eye, _scaleRatio / 100.);
	auto eyeFrame = frame(roi);
	cv::Mat eyeFrameGray;
	cv::cvtColor(eyeFrame, eyeFrameGray, CV_BGR2GRAY);
	cv::equalizeHist(eyeFrameGray, eyeFrameGray);
	cv::Mat eyeFrameBlured;
	cv::blur(eyeFrameGray, eyeFrameBlured, cv::Size(3, 3));

	double minDistance;
	cv::Point minDistance_loc;
	cv::minMaxLoc(eyeFrameBlured, &minDistance, NULL, &minDistance_loc, NULL);
	cv::threshold(eyeFrameBlured, eyeFrameBlured, minDistance, 255, CV_THRESH_BINARY);

	return ic::moveCenter(roi, roi.tl() + minDistance_loc);;
}

void EyeDetector::detectEye(
	const cv::Mat& frame,
	cv::Rect bestFaceObject,
	cv::Rect &leftEye,
	cv::Rect &rightEye,
	const cv::Size& minSize,
	const cv::Size& maxSize
)
{
	auto frameFace = frame(bestFaceObject);
	std::vector<cv::Rect> eyeObjects;
	_eye_cascade.detectMultiScale(frameFace, eyeObjects, 1.02, 2, 0, minSize, maxSize);

	/*cv::Rect bestEyeObject1, bestEyeObject2;
	int minDistanceEyeObjectDinstacne = INT_MAX;
	for (auto o : eyeObjects) {
	auto dist = distance(_lastEyeObject1, o);
	if (dist < minDistanceEyeObjectDinstacne) {
	if (bestEyeObject1.area()) {
	bestEyeObject2 = bestEyeObject1;
	bestEyeObject1 = o;
	}
	else {
	bestEyeObject1 = o;
	}
	minDistanceEyeObjectDinstacne = dist;
	}
	}*/


	//if (bestEyeObject1.area() == 0) bestEyeObject1 = _lastEyeObject1;
	//else _lastEyeObject1 = bestEyeObject1;

	leftEye = cv::Rect(INT_MAX, INT_MAX, INT_MAX, INT_MAX);
	rightEye = cv::Rect();

	bool leftEyeSet = false, rightEyeSet = false;

	for (auto o : eyeObjects) {
		if (o.x < leftEye.x) {
			leftEye = o;
			leftEyeSet = true;
		}
		if (o.x > rightEye.x) {
			rightEye = o;
			rightEyeSet = true;
		}
	}

	if (leftEyeSet) {
		leftEye = findEyeCenter(leftEye, frameFace);
		_lastLeftEyeObject = leftEye;
	}
	else {
		leftEye = _lastLeftEyeObject;
	}

	if (rightEyeSet) {
		rightEye = findEyeCenter(rightEye, frameFace);
		_lastRightEyeObject = rightEye;
	}
	else {
		rightEye = _lastRightEyeObject;
	}
}