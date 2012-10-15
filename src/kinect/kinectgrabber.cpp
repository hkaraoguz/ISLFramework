#include "kinectgrabber.h"

static KinectGrabber* thisGrabber;

KinectGrabber::KinectGrabber(QObject *parent) :
    QObject(parent)
{

    shouldSave = false;

    shouldQuit = false;

    frameCounter = 0;

    thisGrabber = this;
}

sensor_msgs::PointCloud2ConstPtr cloud;



/*void emitFrame(){

    emit frame(cloud);

}*/

//Q_DECLARE_METATYPE()

void KinectGrabber::cb(const sensor_msgs::PointCloud2ConstPtr& input){

   // cloud(new sensor_msgs::PointCloud2ConstPtr);
    //cloud = input;

   // emitFrame();
    emit frame(input);

   // emit thisGrabber->frame(input);

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

   // QString launch= "roslaunch openni_launch openni.launch";

  //  QProcess::execute("killall XnSensorServer");

  //  sleep(3);

   // rosLaunch->start(launch);

   // QProcess::execute("roslaunch", QStringList() << "");

  //  openni->start();

 //   while(!rosLaunch->waitForStarted());


    ros::NodeHandle n;

    // If fails, it returns an empty subscriber
    sub = n.subscribe<sensor_msgs::PointCloud2> ("/camera/depth_registered/points", 1,&KinectGrabber::cb,this);

    if(!sub) emit error();
    else
        emit started();



  /*  while(ros::ok() && !shouldQuit){


    }*/


  //  rosLaunch->terminate();
  //  while(rosLaunch->waitForFinished());

   // n.shutdown();

   // emit finished();


}

void KinectGrabber::stopKinect(){


    sub.shutdown();

}
