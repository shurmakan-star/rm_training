#include "Detector.hpp"
#include <iostream>

Detector::Detector() 
    : dictionary(cv::aruco::getPredefinedDictionary(cv::aruco::DICT_APRILTAG_36h11)),
      detector(dictionary) 
{
}

void Detector::detect(const cv::Mat& inputImage, cv::Mat& binaryImage, 
                      std::vector<std::vector<cv::Point2f>>& corners, 
                      std::vector<int>& ids) 
{
    cv::Mat gray;
  
    cv::cvtColor(inputImage, gray, cv::COLOR_BGR2GRAY);

    cv::adaptiveThreshold(gray, binaryImage, 255, 
                          cv::ADAPTIVE_THRESH_GAUSSIAN_C, 
                          cv::THRESH_BINARY, 11, 2);

    std::vector<std::vector<cv::Point2f>> rejected;
    detector.detectMarkers(gray, corners, ids, rejected);
}