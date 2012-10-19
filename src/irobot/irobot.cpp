#include "irobot.h"
#include <QDebug>
#include <QIODevice>
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
    connect(irobotRunProcess,SIGNAL(error(QProcess::ProcessError)),this,SLOT(handleRobotRunError(QProcess::ProcessError)));

    QString launchCommand = "rosparam set /brown/irobot_create_2_1/port /dev/ttyUSB0";

    irobotSetSerialProcess->setProcessChannelMode(QProcess::MergedChannels);
    irobotSetSerialProcess->start(launchCommand);


    if(irobotSetSerialProcess->waitForFinished(1000)){

        qDebug()<<"iRobot Serial port has been set";
    }

    irobotSetSerialProcess->close();

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

        subs = n.subscribe("sensorPacket",1000,&Irobot::sensorCB,this);

        createPublisher = n.advertise<geometry_msgs::Twist>("cmd_vel",1000);


   return true;
    //qDebug()<<"Ros thread has started";
    }

    return false;

}
Irobot::~Irobot(){

    n.shutdown();

    subs.shutdown();

    if(irobotSetSerialProcess->isOpen()){

        irobotSetSerialProcess->terminate();

        if(irobotSetSerialProcess->waitForFinished(1000));
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
