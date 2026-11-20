#ifndef PNP_SOLVER_HPP
#define PNP_SOLVER_HPP

#include <opencv2/opencv.hpp>
#include <vector>

class PnpSolver
{
public:
    PnpSolver();
    

    void solveAndDraw(cv::Mat& resultImage, 
                      const std::vector<std::vector<cv::Point2f>>& corners,
                      const std::vector<int>& ids);

private:

    cv::Mat cameraMatrix;

    cv::Mat distCoeffs;

    float tagSize;

    void drawCube(cv::Mat& image, const cv::Mat& rvec, const cv::Mat& tvec, float length);

};

#endif 