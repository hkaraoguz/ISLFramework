#include <QThread>
#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <QVector>
class RosThread:public QObject
{
    Q_OBJECT

public:

    RosThread();

    //RosThread(int argc, char **argv, std::string nodeName);

public:


     void shutdownROS();

     geometry_msgs::Twist velocityCommand;

private:
     bool shutdown;

     ros::Publisher velocityCommandPublisher;

signals:

   void  rosStarted();
   void  rosStartFailed();
public slots:

  // void handleVelocityCommand(QVector<double> velCommand);

   void loop();

};
