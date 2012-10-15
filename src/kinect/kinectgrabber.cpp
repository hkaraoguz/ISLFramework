#include "kinectgrabber.h"

KinectGrabber::KinectGrabber(QObject *parent) :
    QObject(parent)
{

}

void KinectGrabber::cb(const sensor_msgs::PointCloud2ConstPtr& input){


    emit frame(input);

    // ROS_INFO("i am here");

   // pcl::fromROSMsg(*input,cld);

   //  if (! viv.wasStopped())
   // viv.showCloud(cld.makeShared());


   // if(shouldQuit)sub.shutdown();

 }

void KinectGrabber::grabFromKinect()
{


    QProcess* rosLaunch = new QProcess();

    rosLaunch->setWorkingDirectory(QDir::temp().absolutePath());
     //  rosLaunch->setProcessChannelMode(QProcess::MergedChannels);

    QString launch= "roslaunch openni_launch openni.launch";

    QProcess::execute("killall XnSensorServer");

    sleep(3);

    rosLaunch->start(launch);

   // QProcess::execute("roslaunch", QStringList() << "");

  //  openni->start();

   if (!rosLaunch->waitForStarted())
           return false;


    ros::NodeHandle n;

    sub = n.subscribe<sensor_msgs::PointCloud2> ("/camera/depth_registered/points", 1, cb);

    if(ros::ok())
        emit started();
    else
        emit error();

    while(ros::ok() && !shouldQuit){


    }


    rosLaunch->terminate();
    while(rosLaunch->waitForFinished());

   // n.shutdown();

    emit finished();


}
