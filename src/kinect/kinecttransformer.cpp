#include "kinecttransformer.h"
#include <QDebug>

//using namespace cv;
using std::vector;
KinectTransformer::KinectTransformer()
{
    translationVector  =  cv::Mat::zeros(3,1,CV_32FC1);

    translationVector.at<float>(0,0) = 1.9985242312092553e-02;

    translationVector.at<float>(1,0) = -7.4423738761617583e-04;

    translationVector.at<float>(2,0) = -1.0916736334336222e-02;


    rotationMatrix = cv::Mat::zeros(3,3,CV_32FC1);

    rotationMatrix.at<float>(0,0) = 9.9984628826577793e-01;

    rotationMatrix.at<float>(0,1) = 1.2635359098409581e-03;

    rotationMatrix.at<float>(0,2) = -1.7487233004436643e-02;

    rotationMatrix.at<float>(1,0) = -1.4779096108364480e-03;

    rotationMatrix.at<float>(1,1) = 9.9992385683542895e-01;

    rotationMatrix.at<float>(1,2) = -1.2251380107679535e-02;

    rotationMatrix.at<float>(2,0) = 1.7470421412464927e-02;

    rotationMatrix.at<float>(2,1) = 1.2275341476520762e-02;

    rotationMatrix.at<float>(2,2) = 9.9977202419716948e-01;

    qDebug()<<"Translation Vector is: "<<translationVector.at<float>(1,0);

    qDebug()<<"Rotation Matrix is: "<<rotationMatrix.at<float>(2,2);

    fx_rgb =5.2921508098293293e+02;
    fy_rgb =5.2556393630057437e+02;
    cx_rgb =3.2894272028759258e+02;
    cy_rgb =2.6748068171871557e+02;
    k1_rgb =2.6451622333009589e-01;
    k2_rgb =-8.3990749424620825e-01;
    p1_rgb =-1.9922302173693159e-03;
    p2_rgb =1.4371995932897616e-03;
    k3_rgb =9.1192465078713847e-01;
}

vector <pointKin> KinectTransformer::transformXYZToImage(cv::Mat xyzImage,  cv::Mat rgbImage){

    //  P3D' = R.P3D + T
    //  P2D_rgb.x = (P3D'.x * fx_rgb / P3D'.z) + cx_rgb
    //  P2D_rgb.y = (P3D'.y * fy_rgb / P3D'.z) + cy_rgb

    vector< pointKin> res;

    for(int i = 0; i < xyzImage.size().width; i++){

        for(int j = 0; j < xyzImage.size().height; j++){

            cv::Vec3f* aPoint = xyzImage.ptr< cv::Vec3f >(j,i);

            cv::Mat aPointMat = cv::Mat::zeros(3,1,CV_32FC1);

            aPointMat.at<float>(0,0) = aPoint->val[0];
             aPointMat.at<float>(1,0) = aPoint->val[1];
              aPointMat.at<float>(2,0) = aPoint->val[2];


            cv::Mat transformedPoint= rotationMatrix*aPointMat + translationVector;

            cv::Point imagePoints; //= Mat::zeros(2,1,CV_16UC1);

            imagePoints.x =(transformedPoint.at<float>(0,0)*this->fx_rgb/transformedPoint.at<float>(2,0)) + this->cx_rgb;

            imagePoints.y = (transformedPoint.at<float>(1,0)*this->fy_rgb/transformedPoint.at<float>(2,0)) + this->cy_rgb;

            if( imagePoints.x >0  &&  imagePoints.x < rgbImage.size().width &&  imagePoints.y > 0 &&  imagePoints.y < rgbImage.size().height){

                    pointKin pt;

                    pt.x = transformedPoint.at<float>(0,0);
                    pt.y = transformedPoint.at<float>(1,0);
                    pt.z = transformedPoint.at<float>(2,0);

                    cv::Vec3b bgr = rgbImage.at< cv::Vec3b >(imagePoints.y,imagePoints.x);

                    pt.b = bgr.val[0];
                    pt.g = bgr.val[1];
                    pt.r =bgr.val[2];

                    pt.imgX = imagePoints.x;

                    pt.imgY = imagePoints.y;

                    res.push_back(pt);

            }
        }

    }

    return res;

}
vector <pointKin> KinectTransformer::transformXYZToImage(pcl::PointCloud<pcl::PointXYZ> xyzCloud, cv::Mat rgbImage){

    //  P3D' = R.P3D + T
    //  P2D_rgb.x = (P3D'.x * fx_rgb / P3D'.z) + cx_rgb
    //  P2D_rgb.y = (P3D'.y * fy_rgb / P3D'.z) + cy_rgb

    vector< pointKin> res;

    for(unsigned long i = 0; i < xyzCloud.size(); i++){

            cv::Mat aPointMat = cv::Mat::zeros(3,1,CV_32FC1);

            aPointMat.at<float>(0,0) = xyzCloud.at(i).x;
            aPointMat.at<float>(1,0) = xyzCloud.at(i).y;
            aPointMat.at<float>(2,0) = xyzCloud.at(i).z;


            cv::Mat transformedPoint= rotationMatrix*aPointMat + translationVector;

            cv::Point imagePoints; //= Mat::zeros(2,1,CV_16UC1);

            imagePoints.x =(transformedPoint.at<float>(0,0)*this->fx_rgb/transformedPoint.at<float>(2,0)) + this->cx_rgb;

            imagePoints.y = (transformedPoint.at<float>(1,0)*this->fy_rgb/transformedPoint.at<float>(2,0)) + this->cy_rgb;

            if( imagePoints.x >0  &&  imagePoints.x < rgbImage.size().width &&  imagePoints.y > 0 &&  imagePoints.y < rgbImage.size().height){

                    pointKin pt;

                    pt.x = transformedPoint.at<float>(0,0);
                    pt.y = transformedPoint.at<float>(1,0);
                    pt.z = transformedPoint.at<float>(2,0);

                    cv::Vec3b bgr = rgbImage.at<cv::Vec3b>(imagePoints.y,imagePoints.x);

                    pt.b = bgr.val[0];
                    pt.g = bgr.val[1];
                    pt.r =bgr.val[2];

                    pt.imgX = imagePoints.x;

                    pt.imgY = imagePoints.y;

                    res.push_back(pt);

            }


    }

    return res;

}
