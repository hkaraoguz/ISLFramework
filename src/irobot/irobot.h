#ifndef IROBOT_H
#define IROBOT_H

#include <QObject>
#include <QFile>
#include <QTextStream>


#include "kinectgrabber.h"

#include <ros/ros.h>
#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Imu.h>
#include <sensor_msgs/PointCloud2.h>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <QApplication>
#include <boost/thread.hpp>
#include <pcl-1.5/pcl/point_types.h>
#include <pcl-1.5/pcl/io/pcd_io.h>
#include <pcl-1.5/pcl/ros/conversions.h>

#include <turtlebot_node/TurtlebotSensorState.h>

//#include <brown_drivers/irobot_create_2_1/msg_gen/cpp/include/irobot_create_2_1/SensorPacket.h>
#include <QProcess>

class Irobot : public QObject
{
    Q_OBJECT
public:

    explicit Irobot(QObject *parent = 0);
    ~Irobot();

    ros::NodeHandle n;

    bool initIrobotConnection(QString robotPortName);

    void setMotion(double forward, double angular);

    void saveData(QFile* file);

    double getFirstOrientation();

    bool isConnected();

private:

    void sensorCB(const turtlebot_node::TurtlebotSensorStateConstPtr& packet);

    void os5000CB(const sensor_msgs::Imu::ConstPtr& packet);

    void kinectCB(const sensor_msgs::PointCloud2::ConstPtr& packet);

    void updateOdometry();

    pcl::PointCloud<pcl::PointXYZRGB> currentCloud;

    geometry_msgs::Quaternion currentOrientation;

    turtlebot_node::TurtlebotSensorStateConstPtr currentSensorPacket;

     ros::Subscriber createSubscriber;

     ros::Subscriber os5000Subscriber;

     ros::Publisher createPublisher;

     geometry_msgs::Twist velCommand;

     bool saveInProgress;

     int frameCount;

     KinectGrabber* kinect;

     double totalTraveledDistanceX;

     double totalTraveledDistanceY;

     double prevDist ;

     double totalAngularHeading;

     double firstOrientation;

     bool connected;
    
signals:
    
public slots:

     void handleRobotRunError(QProcess::ProcessError);

     void handleSetFirstOrientation();


    
};

#endif // IROBOT_H
