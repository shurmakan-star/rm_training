#include <opencv2/opencv.hpp>
#include "Detector.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() 
{
    Detector myDetector;

    // ========================================================
    // 任务 1: 实时识别 AprilTag (打开摄像头)
    // ========================================================
    VideoCapture capTag(0); // 0 代表系统默认摄像头
    if (!capTag.isOpened())
    {
        cout << "错误: 无法打开摄像头，请检查设备连接！" << endl;
        return -1;
    }

    cout << "==================================================" << endl;
    cout << ">>> 开始任务 1: AprilTag 实时识别" << endl;
    cout << ">>> 提示: 在画面中按下 [ESC] 键可退出当前任务并进入魔方识别" << endl;
    cout << "==================================================" << endl;

    Mat frame, resultTagImg;
    
    while (true) 
    {
        capTag >> frame;
        if (frame.empty()) break;

        // detect 内部已自动展示 Image_raw、Image_binary、result 三个窗口
        myDetector.detect(frame, resultTagImg); 

        // 保持 1ms 刷新，按下 ESC 键退出
        if (waitKey(1) == 27) break; 
    }

    // 释放摄像头并关闭窗口，准备进入下一个任务
    capTag.release();
    destroyAllWindows();

    // ========================================================
    // 任务 2: 识别魔方颜色 (播放录制的视频)
    // ========================================================
    cout << "\n==================================================" << endl;
    cout << ">>> 开始任务 2: 魔方视频颜色识别" << endl;
    cout << ">>> 提示: 按下 [ESC] 键退出程序" << endl;
    cout << "==================================================" << endl;
  
    string videoPath = "../resources/cube.mp4"; 
    VideoCapture capCube(videoPath);
    
    if (capCube.isOpened()) 
    {
        Mat resultCubeImg;
        // detect 内部处理视频流并展示 Image_raw、Image_binary、result 三个窗口
        myDetector.detect(capCube, resultCubeImg); 
    } 
    else 
    {
        cout << "错误: 未找到魔方视频，请检查路径: " << videoPath << endl;
    }

    cout << "程序运行结束。" << endl;
    return 0;
}