#ifndef DETECTOR_HPP
#define DETECTOR_HPP

#include <opencv2/opencv.hpp>
#include <string>

using namespace std;
using namespace cv;

class Detector 
{
 public:
    // 识别 AprilTag 并绘制结果
    void detect(const Mat& inputImage, Mat& resultImage);

    // 识别魔方颜色 (根据视频流识别)
    void detect(const VideoCapture& video, Mat& resultImage);

 private:
    // 可以在这里添加一些共用的私有变量或辅助函数
};

#endif // DETECTOR_HPP