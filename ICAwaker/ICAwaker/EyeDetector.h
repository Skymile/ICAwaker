#pragma once

#include "Utils.h"

struct EyeDetector
{
	EyeDetector(std::string haarCascadeFaceFilename, std::string haarCascadeEyeFilename,
		double scaleRatio = 50) :
		_face_cascade(haarCascadeFaceFilename),
		_eye_cascade(haarCascadeEyeFilename),
		_scaleRatio(scaleRatio),
		_lastLeftEyeObject(0, 0, 1, 1),
		_lastRightEyeObject(0, 0, 1, 1)
	{
	}

	double& scaleRatio()
	{
		return _scaleRatio;
	}

	void detect(cv::Mat frame, cv::Rect &face, cv::Rect &leftEye, cv::Rect &rightEye)
	{
		auto bestFaceObject = detectFace(frame);

		if (bestFaceObject.area() != 0) {
			face = bestFaceObject;
			cv::Rect bestLeftEyeObject1, bestRigthEyeObject2;
			detectEye(frame, bestFaceObject, bestLeftEyeObject1, bestRigthEyeObject2);
			leftEye = bestLeftEyeObject1 + bestFaceObject.tl();
			rightEye = bestRigthEyeObject2 + bestFaceObject.tl();
		}
	}

private:
	cv::Rect detectFace(const cv::Mat& frame)
	{
		cv::Mat frameGray;
		cv::cvtColor(frame, frameGray, CV_BGR2GRAY);
		cv::equalizeHist(frameGray, frameGray);

		std::vector<cv::Rect> faceObjects;
		_face_cascade.detectMultiScale(frameGray, faceObjects, 1.03, 2, 0,
			cv::Size(80, 80), cv::Size(150, 150));

		cv::Rect bestFaceObject;
		int minFaceObjectDinstacne = INT_MAX;
		for (auto o : faceObjects) {
			auto dist = distance(_lastFaceObject, o);
			if (dist < minFaceObjectDinstacne) {
				bestFaceObject = o;
				minFaceObjectDinstacne = dist;
			}
		}

		if (bestFaceObject.area() == 0) bestFaceObject = _lastFaceObject;
		else _lastFaceObject = bestFaceObject;

		return bestFaceObject;
	}

	cv::Rect findEyeCenter(cv::Rect eye, const cv::Mat& frame)
	{
		auto roi = scale(eye, _scaleRatio / 100.);
		auto eyeFrame = frame(roi);
		cv::Mat eyeFrameGray;
		cv::cvtColor(eyeFrame, eyeFrameGray, CV_BGR2GRAY);
		cv::equalizeHist(eyeFrameGray, eyeFrameGray);
		cv::Mat eyeFrameBlured;
		cv::blur(eyeFrameGray, eyeFrameBlured, cv::Size(3, 3));

		double min;
		cv::Point min_loc;
		cv::minMaxLoc(eyeFrameBlured, &min, NULL, &min_loc, NULL);
		cv::threshold(eyeFrameBlured, eyeFrameBlured, min, 255, CV_THRESH_BINARY);

		return moveCenter(roi, roi.tl() + min_loc);;
	}

	void detectEye(const cv::Mat& frame, cv::Rect bestFaceObject,
		cv::Rect &leftEye, cv::Rect &rightEye)
	{
		auto frameFace = frame(bestFaceObject);
		std::vector<cv::Rect> eyeObjects;
		_eye_cascade.detectMultiScale(frameFace, eyeObjects, 1.02, 2, 0, cv::Size(30, 30), cv::Size(60, 60));

		/*cv::Rect bestEyeObject1, bestEyeObject2;
		int minEyeObjectDinstacne = INT_MAX;
		for (auto o : eyeObjects) {
		auto dist = distance(_lastEyeObject1, o);
		if (dist < minEyeObjectDinstacne) {
		if (bestEyeObject1.area()) {
		bestEyeObject2 = bestEyeObject1;
		bestEyeObject1 = o;
		}
		else {
		bestEyeObject1 = o;
		}
		minEyeObjectDinstacne = dist;
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


private:
	cv::CascadeClassifier _face_cascade, _eye_cascade;
	cv::Rect _lastFaceObject;
	cv::Rect _lastLeftEyeObject;
	cv::Rect _lastRightEyeObject;
	double _scaleRatio;
};