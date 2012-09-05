#include "kinectutility.h"
#include "kinecttransformer.h"
#include <QDebug>

float RawDepthToMeters(int depthValue)
{
    if (depthValue < 2047)
    {
        return float(1.0 / (double(depthValue) * -0.0030711016 + 3.3309495161));
    }
    return 0.0f;
}

KinectTransformer kinectTransformer;

using std::vector;

KinectUtility::KinectUtility()
{
}
/* Transform the xyz points on the image plane using opencv xyz image */
vector<pointKin> KinectUtility::transformXYZtoImagePlane(cv::Mat xyzImage, cv::Mat rgbImage)
{

    return kinectTransformer.transformXYZToImage(xyzImage,rgbImage);


}
/* Transform the xyz points on the image plane using pcl point cloud */
vector<pointKin> KinectUtility::transformXYZtoImagePlane(pcl::PointCloud<pcl::PointXYZ> xyzCloud, cv::Mat rgbImage){


    return kinectTransformer.transformXYZToImage(xyzCloud,rgbImage);

}

void KinectUtility::convertDepthImage2Cloud(cv::Mat depthImage, sensor_msgs::PointCloud2::Ptr cloud)
{
    static const double fx_d = 1.0 / 5.9421434211923247e+02;
    static const double fy_d = 1.0 / 5.9104053696870778e+02;
    static const double cx_d = 3.3930780975300314e+02;
    static const double cy_d = 2.4273913761751615e+02;

    pcl::PointCloud<pcl::PointXYZRGB> xyzCloud;

   // xyzCloud.width  = depthImage.rows*depthImage.cols;
    xyzCloud.height = 1;
    //xyzCloud.points.resize (xyzCloud.width * xyzCloud.height);

    for(int i = 0; i < depthImage.rows; i++){
        for(int j = 0; j < depthImage.cols; j++){

            float depth = depthImage.at<float>(i,j);

           // qDebug()<<"depth values: "<<depth;

            pcl::PointXYZRGB pt;


           // const double depth = RawDepthToMeters((int)depthValue);

            pt.x = float((j - cx_d) * depth * fx_d);
            pt.y = float((i - cy_d) * depth * fy_d);
            pt.z = float(depth);

            xyzCloud.points.push_back(pt);
        }

        //return result;
    }

    xyzCloud.width  = xyzCloud.points.size();

    pcl::toROSMsg(xyzCloud,*cloud);
}

/*void KinectUtility::saveDatatoXML(vector<point> data, string fileName)
{
    xmlwriter writer(fileName);

    writer.Createtag("points");


    for(int i = 0; i < data.size(); i++){

        writer.Createtag("point");

         point pt = (point)data[i];

         stringstream ss;

         //insert b

         ss<<pt.b;

         writer.CreateChild("b",ss.str());

         ss.str("");

         ss.clear();



       // insert g
         ss<<pt.g;

         writer.CreateChild("g",ss.str());

         ss.str("");

         ss.clear();

       // insert r

         ss<<pt.r;

         writer.CreateChild("r",ss.str());

         ss.str("");

         ss.clear();

      // insert x
         ss<<pt.x;

         writer.CreateChild("x",ss.str());

         ss.str("");

         ss.clear();

      // insert y

         ss<<pt.y;

         writer.CreateChild("y",ss.str());

         ss.str("");

         ss.clear();

      // insert z
         ss<<pt.z;

         writer.CreateChild("z",ss.str());

         ss.str("");

         ss.clear();

      // insert img x
         ss<<pt.imgX;



         writer.CreateChild("imageX",ss.str());

            ss.str("");

            ss.clear();

      // insert img y
           ss<<pt.imgY;

          writer.CreateChild("imageY",ss.str());

          ss.str("");

          ss.clear();

        writer.CloseLasttag();




    }

    writer.CloseAlltags();



}*/
