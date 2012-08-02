#include <QThread>
#include <ros/ros.h>

class RosThread:public QThread
{
    Q_OBJECT

public:

    RosThread();

    RosThread(int argc, char **argv, std::string nodeName);

public:
     void run();

     void shutdownROS();

private:
     bool shutdown;

signals:

   void  rosStarted();
   void  rosStartFailed();

};
