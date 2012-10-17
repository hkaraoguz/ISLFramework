#include "irobot.h"
#include <QDebug>
Irobot::Irobot(QObject *parent) :
    QObject(parent)
{
    irobotSetSerialProcess = new QProcess(this);

    irobotRunProcess = new QProcess(this);

}

void Irobot::sensorCB(const irobot_create_2_1::SensorPacket::ConstPtr& packet){


    qDebug()<<packet->batteryCharge;

    qDebug()<<"Traveled Distance in cms: "<<packet->distance/10;

    qDebug()<<"Angular Rate in degrees: "<<packet->angle;





}

bool Irobot::initIrobotConnection()
{

    QString launchCommand = "rosparam set /brown/irobot_create_2_1/port /dev/ttyUSB0";

    irobotSetSerialProcess->start(launchCommand);

    while(!irobotSetSerialProcess->waitForStarted());

    irobotSetSerialProcess->close();

    launchCommand = "rosrun irobot_create_2_1 driver.py";

    irobotRunProcess->start(launchCommand);

    while(!irobotRunProcess->waitForStarted());

    qDebug()<<"Everything is ok !!!";

    subs = n.subscribe("sensorPacket",1000,&Irobot::sensorCB,this);

    createPublisher = n.advertise<geometry_msgs::Twist>("cmd_vel",1000);


   return true;
    //qDebug()<<"Ros thread has started";


}
Irobot::~Irobot(){

    n.shutdown();

    subs.shutdown();

    if(irobotSetSerialProcess->isOpen()){

        irobotSetSerialProcess->close();

        while(!irobotSetSerialProcess->waitForFinished(1000));

        qDebug()<<"Serial Process has just ended";
    }
    if(irobotRunProcess->isOpen()){

        irobotRunProcess->terminate();

        irobotRunProcess->waitForFinished(3000);

        qDebug()<<"Irobot run process has just ended";


    }

}

void Irobot::setMotion(double forward, double angular){

    velCommand.linear.x = forward;

    velCommand.angular.z = angular;

    createPublisher.publish(velCommand);

}
