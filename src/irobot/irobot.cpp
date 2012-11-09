#include "irobot.h"
#include <QDebug>
#include <QIODevice>
Irobot::Irobot(QObject *parent) :
    QObject(parent)
{
    irobotSetSerialProcess = new QProcess(this);

    irobotRunProcess = new QProcess(this);

    os5000RunProcess = new QProcess(this);

    saveInProgress = false;

    totalTraveledDistanceX = 0;

    totalTraveledDistanceY = 0;


    totalAngularHeading = 0;

    frameCount = 0;

    prevDist = 0;

    currentOrientation.z = 1111;

    // Holds the first orientation that is measured at the beginning position of the robot
    firstOrientation = 0;

}
void Irobot::handleSetFirstOrientation(){


    firstOrientation = currentOrientation.z;

}

void Irobot::sensorCB(const irobot_create_2_1::SensorPacket::ConstPtr& packet){


  //  qDebug()<<packet->batteryCharge/100;

    if(!saveInProgress){
        currentSensorPacket = packet;
        updateOdometry();
}
  //  totalTraveledDistance += currentSensorPacket->distance/10;

  //  totalAngularHeading += currentSensorPacket->angle;

   // qDebug()<<"Traveled Distance in cms: "<<packet->distance/10;

   // qDebug()<<"Angular Rate in degrees: "<<packet->angle;

}
void Irobot::kinectCB(const sensor_msgs::PointCloud2::ConstPtr &packet)
{

    if(!saveInProgress){

        pcl::fromROSMsg(*packet,currentCloud);

        //currentCloud = packet;

    }


}
void Irobot::os5000CB(const sensor_msgs::Imu::ConstPtr &packet){


    if(!saveInProgress)
        currentOrientation = packet->orientation;


}
void Irobot::updateOdometry()
{

    // IF we don't receive compass updates
    if(currentOrientation.z == 1111)
    {

        double x = totalTraveledDistanceX  + ((double)currentSensorPacket->distance/10 - prevDist)*cos(((double)currentSensorPacket->angle)*M_PI/180);

        totalTraveledDistanceX = x;

        double y = totalTraveledDistanceY + ((double)currentSensorPacket->distance/10 - prevDist)*sin((double)currentSensorPacket->angle*M_PI/180);

        totalTraveledDistanceY = y;

        prevDist = (double)currentSensorPacket->distance/10;


    }
    else
    {

        double x = totalTraveledDistanceX  + ((double)currentSensorPacket->distance/10 - prevDist)*cos(((double)currentOrientation.z - firstOrientation)*M_PI/180);

        totalTraveledDistanceX = x;

        double y = totalTraveledDistanceY + (((double)currentSensorPacket->distance/10 - prevDist)*sin(((double)currentOrientation.z - firstOrientation)*M_PI/180));

        totalTraveledDistanceY = y;

        prevDist = (double)currentSensorPacket->distance/10;
    }


}
void Irobot::saveData(QFile* file)
{
    saveInProgress = true;

    QTextStream stream(file);

    // qDebug()<<currentSensorPacket->distance/10;

    // IF we don't receive compass updates
    if(currentOrientation.z == 1111)
    {

        double x = totalTraveledDistanceX  + ((double)currentSensorPacket->distance/10 - prevDist)*cos((double)currentSensorPacket->angle*M_PI/180);

        totalTraveledDistanceX = x;

        double y = totalTraveledDistanceY + (((double)currentSensorPacket->distance/10 - prevDist)*sin((double)currentSensorPacket->angle*M_PI/180));

        totalTraveledDistanceY = y;

        prevDist = (double)currentSensorPacket->distance/10;

        stream<<frameCount<<" "<<x<<" "<<y<<" "<<currentSensorPacket->angle<<" "<<currentOrientation.z<<"\n";
    }
    else
    {

        double x = totalTraveledDistanceX  + ((double)currentSensorPacket->distance/10 - prevDist)*cos((double)currentOrientation.z*M_PI/180);

        totalTraveledDistanceX = x;

        double y = totalTraveledDistanceY + (((double)currentSensorPacket->distance/10 - prevDist)*sin((double)currentOrientation.z*M_PI/180));

        totalTraveledDistanceY = y;

        prevDist = (double)currentSensorPacket->distance/10;

        stream<<frameCount<<" "<<x<<" "<<y<<" "<<currentSensorPacket->angle<<" "<<currentOrientation.z<<"\n";

    }

    frameCount++;

    saveInProgress = false;

}

bool Irobot::initIrobotConnection(QString robotPortName)
{
    connect(irobotRunProcess,SIGNAL(error(QProcess::ProcessError)),this,SLOT(handleRobotRunError(QProcess::ProcessError)));

    QString launchCommand = "rosparam set /brown/irobot_create_2_1/port /dev/";
    launchCommand.append(robotPortName);

    irobotSetSerialProcess->setProcessChannelMode(QProcess::MergedChannels);
    irobotSetSerialProcess->start(launchCommand);


    if(irobotSetSerialProcess->waitForFinished(1000)){

        qDebug()<<"iRobot Serial port has been set";
    }

    irobotSetSerialProcess->close();

    launchCommand = "rosrun os5000 oscompass";

  //  os5000RunProcess->start(launchCommand);

    os5000Subscriber = n.subscribe("os5000_data",10,&Irobot::os5000CB,this);

    launchCommand = "rosrun irobot_create_2_1 driver.py";

    irobotRunProcess->setProcessChannelMode(QProcess::MergedChannels);
    irobotRunProcess->start(launchCommand);

    if(irobotRunProcess->waitForFinished(3000)){

        QByteArray arr;

        arr.append(irobotRunProcess->readAll());

      //  QString str;

       // str.fromUtf8(arr.data());

        qDebug(arr.data());


        qDebug()<<"No irobot connection";

        irobotRunProcess->close();
    }
    else{

        qDebug()<<"Everything is ok !!!";

        createSubscriber = n.subscribe("sensorPacket",100,&Irobot::sensorCB,this);

        createPublisher = n.advertise<geometry_msgs::Twist>("cmd_vel",100);


        return true;

    }

    return false;

}
Irobot::~Irobot(){

    n.shutdown();

    createSubscriber.shutdown();

    if(irobotSetSerialProcess->isOpen()){

        irobotSetSerialProcess->terminate();

        if(irobotSetSerialProcess->waitForFinished(1000))
            qDebug()<<"Serial Process has just ended";
    }
    if(irobotRunProcess->isOpen()){

        irobotRunProcess->terminate();

        if(irobotRunProcess->waitForFinished(3000))
                qDebug()<<"Irobot run process has just ended";


    }

}

void Irobot::setMotion(double forward, double angular){

    velCommand.linear.x = forward;

    velCommand.angular.z = angular;

    createPublisher.publish(velCommand);

}

void Irobot::handleRobotRunError(QProcess::ProcessError error){

    qDebug()<<error;
}
double Irobot::getFirstOrientation()
{

    return firstOrientation;

}
