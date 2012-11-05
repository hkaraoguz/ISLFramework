#include "kinectgrabber.h"
#include <rosbag/recorder.h>
#include <rosbag/bag.h>

#include <boost/thread.hpp>
static KinectGrabber* thisGrabber;

KinectGrabber::KinectGrabber(QObject *parent) :
    QObject(parent)
{

    shouldSave = false;

    shouldQuit = false;

    frameCounterImage = 0;

    frameCounterCloud = 0;

    saveCloud = false;

    saveImage = false;

    thisGrabber = this;

    emitCloudFrame = false;


}

sensor_msgs::PointCloud2ConstPtr cloud;



/*void emitFrame(){

    emit frame(cloud);

}*/

//Q_DECLARE_METATYPE()

void KinectGrabber::cb(const sensor_msgs::PointCloud2ConstPtr& input){

  //  std::cout<<"2 "<<boost::this_thread::get_id()<<std::endl;
    //rosbag::Bag bag;

    //bag.open("irobot_bag",rosbag::bagmode::Write);

    //bag.write("/camera/depth_registered/points",ros::Time::now(),input);


   // cloud(new sensor_msgs::PointCloud2ConstPtr);
    //cloud = input;

   // emitFrame();

    if(this->emitCloudFrame)
        emit frame(input);

   // emit thisGrabber->frame(input);

    // ROS_INFO("i am here");

    if(saveCloud){

        pcl::PointCloud<pcl::PointXYZRGB> cld;

        pcl::fromROSMsg(*input,cld);

        QString fileName = "cloud_";

        fileName.append(QString::number(frameCounterCloud));

        fileName.append(".pcd");

        pcl::io::savePCDFileBinary(fileName.toStdString(),cld);

        frameCounterCloud++;

        saveCloud = false;
    }
   //  if (! viv.wasStopped())
   // viv.showCloud(cld.makeShared());


   // if(shouldQuit)sub.shutdown();

 }
void KinectGrabber::imagecb(const sensor_msgs::ImageConstPtr &input){

   //  std::cout<<"1 "<<boost::this_thread::get_id()<<std::endl;

    cv_bridge::CvImagePtr img = cv_bridge::toCvCopy(input);

    emit imageFrame(img->image);

    if(saveImage)
    {

        QString fileName = "rgbImage_";

        fileName.append(QString::number(frameCounterImage));

        fileName.append(".jpg");

        cv::imwrite(fileName.toStdString(),img->image);

        frameCounterImage++;

        saveImage = false;

    }
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

    imageSub = n.subscribe<sensor_msgs::Image>("/camera/rgb/image_color",1,&KinectGrabber::imagecb,this);

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

void KinectGrabber::handleSaveRequest(){

    saveCloud = true;
    saveImage = true;

}

void KinectGrabber::setEmitCloudFrame(bool status){

    emitCloudFrame = status;

}
