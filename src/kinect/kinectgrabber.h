#ifndef KINECTGRABBER_H
#define KINECTGRABBER_H

#include <QObject>

#include <ros/ros.h>

#include <pcl-1.5/pcl/ros/conversions.h>
#include <pcl-1.5/pcl/point_cloud.h>
#include <pcl-1.5/pcl/point_types.h>
#include <pcl-1.5/pcl/visualization/cloud_viewer.h>
#include <pcl-1.5/pcl/visualization/pcl_visualizer.h>
#include <pcl-1.5/pcl/io/pcd_io.h>

#include <ros/console.h>

#include <sensor_msgs/PointCloud2.h>

#include<std_msgs/String.h>

#include <QDebug>
#include <QProcess>
#include <QDir>

//class pcl;

class KinectGrabber : public QObject
{
    Q_OBJECT
public:
    explicit KinectGrabber(QObject *parent = 0);

    void cb(const sensor_msgs::PointCloud2ConstPtr& input);

    void imagecb(const sensor_msgs::ImageConstPtr& input);

    void stopKinect();

  //  static void catchKeyboardEvent(const pcl::visualization::KeyboardEvent& evnt, void* viewer_void);

  //  pcl::visualization::CloudViewer viv;

    pcl::PointCloud<pcl::PointXYZRGB> cld;

    bool shouldSave;

    bool shouldQuit;

    int frameCounter;

    ros::Subscriber sub;

    ros::Subscriber imageSub;
private:

    bool saveImage;
    bool saveCloud;
    
signals:

    void started();

    void finished();

    void error();

    void frame(const sensor_msgs::PointCloud2ConstPtr& cloud);
    
public slots:

    void handleSaveRequest();

     void grabFromKinect();


    
};

#endif // KINECTGRABBER_H
