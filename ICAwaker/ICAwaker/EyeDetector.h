#pragma once

#include "Utils.h"

struct EyeDetector
{
	EyeDetector(std::string haarCascadeFaceFilename, std::string haarCascadeEyeFilename,
		double scaleRatio = 30) :
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

	bool detect(cv::Mat frame, cv::Rect &face, cv::Rect &leftEye, cv::Rect &rightEye,
		bool &faceDetected, bool &leftEyeDetected, bool &rightEyeDetected)
	{
		_frameSize.y = frame.cols;
		_frameSize.x = frame.rows;
		if (faceDetected = detectFace(frame, face)) {
			auto eyesDetected = detectEye(frame, face, leftEye, rightEye,
				leftEyeDetected, rightEyeDetected);
			leftEye += face.tl();
			rightEye += face.tl();

			return eyesDetected;
		}
		leftEyeDetected = rightEyeDetected = false;

		return false;
	}

private:
	bool detectFace(const cv::Mat& frame, cv::Rect &faceObject)
	{
		cv::Mat frameGray;
		cv::cvtColor(frame, frameGray, CV_BGR2GRAY);
		//cv::equalizeHist(frameGray, frameGray);

		std::vector<cv::Rect> faceObjects;
		_face_cascade.detectMultiScale(frameGray, faceObjects, 1.04, 3, 0,
			cv::Size(80, 80), cv::Size(150, 150));


		int minFaceObjectDinstacne = INT_MAX;
		for (auto o : faceObjects) {
			auto dist = distance(_lastFaceObject, o);
			if (dist < minFaceObjectDinstacne) {
				faceObject = o;
				minFaceObjectDinstacne = dist;
			}
		}

		if (faceObject.empty()) {
			faceObject = _lastFaceObject;
			return false;
		}
		else _lastFaceObject = faceObject;

		return true;
	}

	void detetEyeBlink(cv::Mat frameFace, cv::Mat frame, cv::Rect roi, double min)
	{
		cv::Mat inputFrame;
		frameFace(roi).copyTo(inputFrame);

		auto output = inputFrame;
		cv::GaussianBlur(output, output, cv::Size(5, 5), 0);
		cv::cvtColor(output, output, CV_BGR2GRAY);
		cv::equalizeHist(output, output);
		cv::threshold(output, output, min + 10, 255, CV_THRESH_BINARY);
		//output = cv::Scalar(255) - output;

		std::vector<cv::Vec4i> hierarchy;
		std::vector<std::vector<cv::Point>> contours;
		cv::findContours(output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);

		static cv::RNG rng(6456);

		cv::Mat outputImage = cv::Mat::zeros(output.size(), CV_8UC3);
		cv::drawContours(outputImage, contours, contours.size() - 1, cv::Scalar(255, 255, 0), 1, 8, hierarchy, 0);

		//std::vector<cv::Rect> boundRect(contours.size());
		auto boundRect = cv::boundingRect(cv::Mat(contours.back()));
		auto ratio = 1.f * boundRect.width / boundRect.height;

		cv::imshow("debug", outputImage);
		std::stringstream ss;
		ss << std::setprecision(2) << std::fixed << ratio;

		cv::putText(frameFace, ss.str(), cv::Point(5, 20), CV_FONT_HERSHEY_SIMPLEX,
			0.8, cv::Scalar(128, 128, 0));

		if (ratio > 1.5f) {
			cv::putText(frameFace, "blink", cv::Point(5, 40), CV_FONT_HERSHEY_SIMPLEX,
				0.8, cv::Scalar(0, 128, 128));
		}

		/*cv::SimpleBlobDetector::Params detectorParams{};
		//detectorParams.filterByArea = true;
		detectorParams.minArea = 5;
		detectorParams.maxArea = 500;

		detectorParams.filterByCircularity = true;
		detectorParams.minCircularity = 0.68;

		//detectorParams.filterByConvexity = true;
		//detectorParams.minConvexity = 0.87;

		//detectorParams.filterByInertia = true;
		//detectorParams.minInertiaRatio = 0.01;
		//detectorParams.minDistBetweenBlobs = 5;
		//detectorParams.filterByColor = false;

		auto detector = cv::SimpleBlobDetector::create(detectorParams);
		std::vector<cv::KeyPoint> keyPoints;
		detector->detect(output, keyPoints);

		cv::Mat colorOutput;
		cv::cvtColor(output, colorOutput, CV_GRAY2BGR);

		//for (auto &k : keyPoints) {

		//}

		cv::drawKeypoints(colorOutput, keyPoints, colorOutput, cv::Scalar(255, 0, 0));

		//cv::createTrackbar("threshold", "debug", &threshold, 500);
		cv::imshow("debug", colorOutput);

		if (keyPoints.empty()) {
			cv::putText(frame, "blink", cv::Point(5, 20), CV_FONT_HERSHEY_SIMPLEX,
				0.8, cv::Scalar(0, 0, 255));
		}*/
	}

	cv::Rect findEyeCenter(cv::Rect eye, const cv::Mat& frame, cv::Rect& roi, cv::Point& minLoc, double& min)
	{
		roi = scale(eye, _scaleRatio / 100.);
		auto eyeFrame = frame(roi);
		cv::Mat eyeFrameGray;
		cv::cvtColor(eyeFrame, eyeFrameGray, CV_BGR2GRAY);
		cv::equalizeHist(eyeFrameGray, eyeFrameGray);
		cv::Mat eyeFrameBlured;
		cv::blur(eyeFrameGray, eyeFrameBlured, cv::Size(3, 3));

		cv::minMaxLoc(eyeFrameBlured, &min, NULL, &minLoc, NULL);

		return moveCenter(roi, roi.tl() + minLoc);
	}

	bool detectEye(const cv::Mat& frame, cv::Rect bestFaceObject, cv::Rect &leftEye,
		cv::Rect &rightEye, bool &leftEyeDetected, bool &rightEyeDetected)
	{
		auto frameFace = frame(bestFaceObject);
		/*
		binaryzacja? normalizacja?
		*/
		std::vector<cv::Rect> eyeObjects;
		_eye_cascade.detectMultiScale(frameFace, eyeObjects, 1.04, 3, 0, cv::Size(30, 30), cv::Size(60, 60));

		leftEye = cv::Rect(INT_MAX, INT_MAX, INT_MAX, INT_MAX);
		rightEye = cv::Rect();

		leftEyeDetected = false;
		rightEyeDetected = false;

		for (auto o : eyeObjects) {
			if (o.x < leftEye.x) {
				leftEye = o;
				leftEyeDetected = true;
			}
			if (o.x > rightEye.x) {
				rightEye = o;
				rightEyeDetected = true;
			}
		}

		if (leftEyeDetected) {
			cv::Rect roi;
			cv::Point minLoc;
			double min;
			leftEye = findEyeCenter(leftEye, frameFace, roi, minLoc, min);
			detetEyeBlink(frameFace, frame, moveCenter(roi, roi.tl() + minLoc), min);
			_lastLeftEyeObject = leftEye;
		}
		else {
			leftEye = _lastLeftEyeObject;
		}

		if (rightEyeDetected) {
			cv::Rect roi;
			cv::Point minLoc;
			double min;
			rightEye = findEyeCenter(rightEye, frameFace, roi, minLoc, min);
			_lastRightEyeObject = rightEye;
		}
		else {
			rightEye = _lastRightEyeObject;
		}

		return leftEyeDetected && rightEyeDetected;
	}


private:
	cv::CascadeClassifier _face_cascade, _eye_cascade;
	cv::Rect _lastFaceObject;
	cv::Rect _lastLeftEyeObject;
	cv::Rect _lastRightEyeObject;
	cv::Point _frameSize;
	double _scaleRatio;
};