#include "rosThread.h"
#include <QDebug>
RosThread::RosThread()
{
    shutdown = false;

    velocityCommand.linear.x = 0;
    velocityCommand.angular.z = 0;

}

void RosThread::loop()
{
    ros::NodeHandle n;

   velocityCommandPublisher =  n.advertise<geometry_msgs::Twist>("cmd_vel",1);

    if(!ros::ok()){

        emit rosStartFailed();

        return;
    }

     emit rosStarted();

    // ros::AsyncSpinner spinner(2);

    // spinner.start();

    ros::Rate loop(10);

    while(ros::ok())
    {

        velocityCommandPublisher.publish(velocityCommand);

        ros::spinOnce();

        loop.sleep();



    }





}
void RosThread::shutdownROS()
{
    ros::shutdown();
   // shutdown = true;


}
/*void RosThread::handleVelocityCommand(QVector<double> velCommand)
{
     qDebug()<<"Helllo"<<velCommand.size();

    if(velCommand.size() == 2){

        qDebug()<<"Helllo"<<velCommand.size();


        velocityCommand.linear.x = (double)velCommand.at(0);
        velocityCommand.angular.z = (double)velCommand.at(1);

        qDebug()<<"Hello"<<velocityCommand.linear.x<<" "<<velocityCommand.angular.z;
    }
    else
    {
        velocityCommand.linear.x = 0.0;
        velocityCommand.angular.z = 0.0;

    }
}*/

