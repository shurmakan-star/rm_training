#include <opencv2/opencv.hpp>
#include <iostream>
#include "Detector.hpp"
#include "pnp_solver.hpp"

int main() {
   
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cout << "无法打开摄像头" << std::endl;
        return -1;
    }

    
    Detector detector;
    PnpSolver pnpSolver;

    cv::Mat frame, binaryImage, resultImage;
    std::vector<std::vector<cv::Point2f>> corners;
    std::vector<int> ids;

    while (true) 
    {
        cap >> frame;
        if (frame.empty()) break;

        resultImage = frame.clone();

        corners.clear();
        ids.clear();
        detector.detect(frame, binaryImage, corners, ids);

        pnpSolver.solveAndDraw(resultImage, corners, ids);

        cv::imshow("Image_raw", frame);
        cv::imshow("Image_binary", binaryImage);
        cv::imshow("result", resultImage);

        if (cv::waitKey(30) == 27) {
            break;
        }
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}