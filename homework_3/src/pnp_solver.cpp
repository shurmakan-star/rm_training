#include "pnp_solver.hpp"
#include <iostream>
#include <stdexcept> 
#include <opencv2/geometry.hpp> 

PnpSolver::PnpSolver() 
{
    std::string configPath = "../config/camera_params.yaml";
    cv::FileStorage fs(configPath, cv::FileStorage::READ);

    if (!fs.isOpened()) 
    {
        std::cerr << "无法打开配置文件: " << configPath << std::endl;
        throw std::runtime_error("Missing config file");
    }

    
    fs["camera_matrix"] >> cameraMatrix;
    fs["dist_coeffs"] >> distCoeffs;
    fs["tag_size"] >> tagSize;

    fs.release(); 

    if (cameraMatrix.empty() || distCoeffs.empty() || tagSize <= 0.0f) {
        std::cerr << "[错误] 配置文件格式不正确，缺少 camera_matrix, dist_coeffs 或 tag_size" << std::endl;
        throw std::runtime_error("Invalid config file");
    }

    std::cout << "当前读取的 Tag 尺寸: " << tagSize << " 米" << std::endl;
    
}

void PnpSolver::solveAndDraw(cv::Mat& resultImage, 
                             const std::vector<std::vector<cv::Point2f>>& corners,
                             const std::vector<int>& ids) 
{
    if (ids.empty()) return;

    std::vector<cv::Point3f> objectPoints = {
        cv::Point3f(-tagSize / 2, -tagSize / 2, 0.0f), 
        cv::Point3f( tagSize / 2, -tagSize / 2, 0.0f), 
        cv::Point3f( tagSize / 2,  tagSize / 2, 0.0f), 
        cv::Point3f(-tagSize / 2,  tagSize / 2, 0.0f) 
    };

    cv::aruco::drawDetectedMarkers(resultImage, corners, cv::noArray());

       for (size_t i = 0; i < ids.size(); i++) {
        const auto& imagePoints = corners[i];

        cv::Mat rvec, tvec;
     
bool success = cv::solvePnP(objectPoints, imagePoints, cameraMatrix, distCoeffs, rvec, tvec, false, cv::SOLVEPNP_ITERATIVE);
        
        if (success)
        {
            drawCube(resultImage, rvec, tvec, tagSize);
        }
         else
        {
            std::cout << "解算失败" << std::endl;
        }
    }
}

void PnpSolver::drawCube(cv::Mat& image, const cv::Mat& rvec, const cv::Mat& tvec, float length) {
    float half_l = length / 2.0f;

    std::vector<cv::Point3f> cubePoints = 
    {
        cv::Point3f(-half_l, -half_l, 0.0f),
        cv::Point3f( half_l, -half_l, 0.0f),
        cv::Point3f( half_l,  half_l, 0.0f),
        cv::Point3f(-half_l,  half_l, 0.0f),
  
        cv::Point3f(-half_l, -half_l, -length),
        cv::Point3f( half_l, -half_l, -length),
        cv::Point3f( half_l,  half_l, -length),
        cv::Point3f(-half_l,  half_l, -length)
    };

    std::vector<cv::Point2f> projectedPoints;
    cv::projectPoints(cubePoints, rvec, tvec, cameraMatrix, distCoeffs, projectedPoints);

    cv::line(image, projectedPoints[0], projectedPoints[1], cv::Scalar(0, 255, 0), 2);
    cv::line(image, projectedPoints[1], projectedPoints[2], cv::Scalar(0, 255, 0), 2);
    cv::line(image, projectedPoints[2], projectedPoints[3], cv::Scalar(0, 255, 0), 2);
    cv::line(image, projectedPoints[3], projectedPoints[0], cv::Scalar(0, 255, 0), 2);
  
    cv::line(image, projectedPoints[4], projectedPoints[5], cv::Scalar(255, 0, 0), 2);
    cv::line(image, projectedPoints[5], projectedPoints[6], cv::Scalar(255, 0, 0), 2);
    cv::line(image, projectedPoints[6], projectedPoints[7], cv::Scalar(255, 0, 0), 2);
    cv::line(image, projectedPoints[7], projectedPoints[4], cv::Scalar(255, 0, 0), 2);

    cv::line(image, projectedPoints[0], projectedPoints[4], cv::Scalar(0, 0, 255), 2);
    cv::line(image, projectedPoints[1], projectedPoints[5], cv::Scalar(0, 0, 255), 2);
    cv::line(image, projectedPoints[2], projectedPoints[6], cv::Scalar(0, 0, 255), 2);
    cv::line(image, projectedPoints[3], projectedPoints[7], cv::Scalar(0, 0, 255), 2);
}