#include "rosThread.h"

RosThread::RosThread()
{
    shutdown = false;


}

RosThread::RosThread(int argc, char **argv, std::string nodeName){

    //  ros::init(argc, argv, nodeName);


}

void RosThread::run(){


    if(!ros::ok()){

        emit rosStartFailed();

        return;
    }

     emit rosStarted();

    ros::Rate loop(30);

    while(ros::ok() &&  !shutdown){



                ros::spinOnce();

                loop.sleep();



    }





}
void RosThread::shutdownROS()
{

    shutdown = true;


}
