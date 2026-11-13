#include "Detector.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <opencv2/geometry.hpp> 

void Detector::detect(const Mat& inputImage, Mat& resultImage) 
{
    resultImage = inputImage.clone();
    if (resultImage.empty()) return;

    Mat gray, binary;
    cvtColor(inputImage, gray, COLOR_BGR2GRAY);

    adaptiveThreshold(gray, binary, 255, ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 11, 2);


    aruco::Dictionary dictionary = aruco::getPredefinedDictionary(aruco::DICT_APRILTAG_36h11);
    aruco::ArucoDetector detector(dictionary);
    
    vector<int> ids;
    vector<vector<Point2f>> corners;
    detector.detectMarkers(resultImage, corners, ids);

    if (!ids.empty()) 
    {
 
        aruco::drawDetectedMarkers(resultImage, corners, noArray(), Scalar(0, 255, 0));
        
        for (size_t i = 0; i < ids.size(); i++) 
        {
            Point2f center(0, 0);
            for (int j = 0; j < 4; j++) 
            {
                center.x += corners[i][j].x / 4.0f;
                center.y += corners[i][j].y / 4.0f;
            }
            
     
            int physicalId = ids[i] + 1;
            string text = to_string(physicalId);
            

            int baseline = 0;
            double fontScale = 1.3;
            int thickness = 3;
            Size textSize = getTextSize(text, FONT_HERSHEY_SIMPLEX, fontScale, thickness, &baseline);
            Point textOrigin((int)(center.x - textSize.width / 2), (int)(center.y + textSize.height / 2));
            

            putText(resultImage, text, textOrigin, FONT_HERSHEY_SIMPLEX, fontScale, Scalar(0, 0, 255), thickness);
            
            cout << "Detected AprilTag: " << physicalId << endl;
        }
    }


    imshow("Image_raw", inputImage);
    imshow("Image_binary", binary);
    imshow("result", resultImage);
}


struct CubeColorDef {
    string name;
    Scalar drawColor;
    Scalar lowerHSV1;
    Scalar upperHSV1;
    Scalar lowerHSV2;
    Scalar upperHSV2;
    bool hasSecondRange;
};

void Detector::detect(const VideoCapture& video, Mat& resultImage) {
    VideoCapture& cap = const_cast<VideoCapture&>(video);
    Mat frame;
    
    if (!cap.isOpened()) {
        cout << "Error: 无法打开魔方视频文件！" << endl;
        return;
    }

    vector<CubeColorDef> colorDefs = {
        {"White", Scalar(255, 255, 255), 
         Scalar(75, 0, 50), Scalar(112, 58, 255), 
         Scalar(), Scalar(), false},
        
        {"Blue", Scalar(255, 0, 0), 
         Scalar(62, 75, 50), Scalar(120, 255, 255), 
         Scalar(), Scalar(), false},

        {"Red", Scalar(0, 0, 255), 
         Scalar(0, 140, 65), Scalar(10, 255, 255), 
         Scalar(156, 140, 65), Scalar(179, 255, 255), true},

        {"Orange", Scalar(0, 140, 255), 
         Scalar(11, 75, 25), Scalar(23, 255, 255), 
         Scalar(), Scalar(), false},

        {"Yellow", Scalar(0, 255, 255), 
         Scalar(24, 90, 65), Scalar(50, 255, 255), 
         Scalar(), Scalar(), false}
    };

    Mat kernel = getStructuringElement(MORPH_RECT, Size(17, 17));

    while (true)
     {
        cap >> frame;
        if (frame.empty()) break;

        resultImage = frame.clone();
        Mat imgHSV;
        cvtColor(frame, imgHSV, COLOR_BGR2HSV);
        medianBlur(imgHSV, imgHSV, 5);

        Mat bestBinaryMask = Mat::zeros(frame.size(), CV_8UC1);
        string detectedColor = "Unknown";
        Scalar boxColor = Scalar(0, 255, 0);
        Rect bestRect;
        double maxAreaFound = 0;

        for (const auto& color : colorDefs) 
        {
            Mat mask1, mask2, totalMask;
            inRange(imgHSV, color.lowerHSV1, color.upperHSV1, mask1);
            if (color.hasSecondRange) 
            {
                inRange(imgHSV, color.lowerHSV2, color.upperHSV2, mask2);
                totalMask = mask1 | mask2;
            } 
            else 
            {
                totalMask = mask1;
            }

            Mat dilatedMask;
            dilate(totalMask, dilatedMask, kernel);

            vector<vector<Point>> contours;
            findContours(dilatedMask, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

            for (const auto& cnt : contours) 
            {
                double area = contourArea(cnt);
                if (area < 3500 || area > frame.cols * frame.rows * 0.40) continue;

                Rect r = boundingRect(cnt);
                float aspect = (float)r.width / (float)r.height;

                if (aspect >= 0.70f && aspect <= 1.45f &&r.width < frame.cols * 0.70 && r.height < frame.rows * 0.70) 
                {
                    if (area > maxAreaFound) 
                    {
                        maxAreaFound = area;
                        detectedColor = color.name;
                        boxColor = color.drawColor;
                        bestRect = r;
                        bestBinaryMask = dilatedMask;
                    }
                }
            }
        }

        if (maxAreaFound > 0)
         {
            rectangle(resultImage, bestRect, boxColor, 3);

            Point textPos(bestRect.x, max(bestRect.y - 10, 25));
            putText(resultImage, detectedColor, textPos, FONT_HERSHEY_SIMPLEX, 1.0, boxColor, 2);
        }

        imshow("Image_raw", frame);
        imshow("Image_binary", bestBinaryMask); 
        imshow("result", resultImage);

        if (waitKey(30) == 27) break;
    }
}