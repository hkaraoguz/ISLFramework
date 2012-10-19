#ifndef IROBOT_H
#define IROBOT_H

#include <QObject>

#include <ros/ros.h>

#include <std_msgs/String.h>
#include <geometry_msgs/Twist.h>
#include <sstream>
#include <opencv2/opencv.hpp>
#include <QApplication>
#include <boost/thread.hpp>

#include <brown_drivers/irobot_create_2_1/msg_gen/cpp/include/irobot_create_2_1/SensorPacket.h>
#include <QProcess>

class Irobot : public QObject
{
    Q_OBJECT
public:

    explicit Irobot(QObject *parent = 0);

    ~Irobot();

    void sensorCB(const irobot_create_2_1::SensorPacket::ConstPtr& packet);
    ros::NodeHandle n;

    bool initIrobotConnection();

    QProcess* irobotSetSerialProcess;

    QProcess* irobotRunProcess;

    void setMotion(double forward, double angular);
private:
     ros::Subscriber subs;

     ros::Publisher createPublisher;

     geometry_msgs::Twist velCommand;
    
signals:
    
public slots:

     void handleRobotRunError(QProcess::ProcessError);


    
};

#endif // IROBOT_H
