#include <opencv2/opencv.hpp>
#include "Detector.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() 
{
    Detector myDetector;

    VideoCapture capTag(0); 
    if (!capTag.isOpened())
    {
        cout << "错误: 无法打开摄像头，请检查设备连接！" << endl;
        return -1;
    }

    Mat frame, resultTagImg;
    
    while (true) 
    {
        capTag >> frame;
        if (frame.empty()) break;

        myDetector.detect(frame, resultTagImg); 

        if (waitKey(1) == 27) break; 
    }

    capTag.release();
    destroyAllWindows();

    string videoPath = "../resources/cube.mp4"; 
    VideoCapture capCube(videoPath);
    
    if (capCube.isOpened()) 
    {
        Mat resultCubeImg;

        myDetector.detect(capCube, resultCubeImg); 
    } 
    else 
    {
        cout << "错误: 未找到魔方视频，请检查路径: " << videoPath << endl;
    }

    cout << "程序运行结束。" << endl;
    return 0;
}