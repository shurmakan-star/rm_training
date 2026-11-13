#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

class Detector 
{
 public:
    void detect(const Mat& inputImage, Mat& resultImage);


    void detect(const VideoCapture& video, Mat& resultImage);

 private:

};

#endif