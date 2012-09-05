#ifndef KINECTUTILITY_H
#define KINECTUTILITY_H


//#include "xmlwriter.h"
#include "pcprocessing.h"
#include <vector>

/*#include "imageprocess.h"
#include <pcl-1.5/pcl/point_types.h>
#include <pcl-1.5/pcl/point_types.h>
#include <pcl-1.5/pcl/io/pcd_io.h>*/

using std::vector;

// Point structure to hold the xyrgb values
struct pointKin{

    int r;
    int g;
    int b;

    float x;
    float y;
    float z;

    int imgX;
    int imgY;

};



class KinectUtility
{
public:
    KinectUtility();

  //  static void saveDatatoXML(vector<point> data, string fileName);

    static vector<pointKin> transformXYZtoImagePlane(cv::Mat xyzImage, cv::Mat rgbImage);

    static vector<pointKin> transformXYZtoImagePlane(pcl::PointCloud<pcl::PointXYZ> xyzCloud, cv::Mat rgbImage);

    static void convertDepthImage2Cloud(cv::Mat depthImage, sensor_msgs::PointCloud2::Ptr cloud);






};

#endif // KINECTUTILITY_H
