#include "rosThread.h"

RosThread::RosThread()
{
    shutdown = false;

    int argc; // Player Main() method does not take argument
    char **argv; // What to do with argc and argv??

   // const M_string nnn;

    ros::init(argc,argv,"ISLFramework");
}

RosThread::RosThread(int argc, char **argv, std::string nodeName){

    //  ros::init(argc, argv, nodeName);

    ros::init(argc,argv,nodeName);
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
    ros::shutdown();
    shutdown = true;


}
