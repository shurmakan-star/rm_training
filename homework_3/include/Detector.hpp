#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <vector>

class Detector 
{
public:
    Detector();
    void detect(const cv::Mat& inputImage, cv::Mat& binaryImage, 
                std::vector<std::vector<cv::Point2f>>& corners, 
                std::vector<int>& ids);

private:
    cv::aruco::Dictionary dictionary;
    cv::aruco::ArucoDetector detector;
};

#endif 