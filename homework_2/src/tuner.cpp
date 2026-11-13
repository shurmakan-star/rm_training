#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int hmin = 0, smin = 0, vmin = 0;
int hmax = 179, smax = 255, vmax = 255;

int main() {
    string videoPath = "../resources/cube.mp4"; 
    VideoCapture cap(videoPath);
    if (!cap.isOpened())
     {
        cout << "无法打开视频" << endl;
        return -1;
    }

    namedWindow("Trackbars", WINDOW_AUTOSIZE);
    createTrackbar("H Min", "Trackbars", &hmin, 179);
    createTrackbar("H Max", "Trackbars", &hmax, 179);
    createTrackbar("S Min", "Trackbars", &smin, 255);
    createTrackbar("S Max", "Trackbars", &smax, 255);
    createTrackbar("V Min", "Trackbars", &vmin, 255);
    createTrackbar("V Max", "Trackbars", &vmax, 255);

    Mat frame, imgHSV, mask, result;
    bool pause = false;
    while (true) 
    {
        if (!pause) 
        {
            cap >> frame;
            if (frame.empty()) 
            {
                cap.set(CAP_PROP_POS_FRAMES, 0); 
                continue;
            }
        }

        cvtColor(frame, imgHSV, COLOR_BGR2HSV);
        medianBlur(imgHSV, imgHSV, 5);

        Scalar lower(hmin, smin, vmin);
        Scalar upper(hmax, smax, vmax);
        inRange(imgHSV, lower, upper, mask);

       
        bitwise_and(frame, frame, result, mask);

        imshow("Video Frame", frame);
        imshow("Mask (二值化)", mask);
        imshow("Color Extracted (提取结果)", result);

        char key = (char)waitKey(30);
        if (key == 27) break; // ESC 退出
        if (key == ' ') pause = !pause; // 空格暂停/播放
        if (key == 'p' || key == 'P') { // 打印参数
            cout << "\n>>> 记录参数: " << endl;
            cout << "Scalar(" << hmin << ", " << smin << ", " << vmin << "), "
                 << "Scalar(" << hmax << ", " << smax << ", " << vmax << ")" << endl;
        }
    }

    return 0;
}