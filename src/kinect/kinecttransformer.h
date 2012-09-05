#ifndef KINECTTRANSFORMER_H
#define KINECTTRANSFORMER_H
#include "kinectutility.h"

class KinectTransformer
{
public:
    KinectTransformer();

    std::vector < pointKin > transformXYZToImage(cv::Mat xyzImage, cv::Mat rgbImage);

    std::vector < pointKin > transformXYZToImage(pcl::PointCloud<pcl::PointXYZ> xyzCloud, cv::Mat rgbImage);

private:

    cv::Mat translationVector;

    cv::Mat rotationMatrix;

    double fx_rgb;
    double fy_rgb;
    double cx_rgb;
    double cy_rgb;
    double k1_rgb;
    double k2_rgb;
    double p1_rgb;
    double p2_rgb;
    double k3_rgb;


};

#endif // KINECTTRANSFORMER_H
