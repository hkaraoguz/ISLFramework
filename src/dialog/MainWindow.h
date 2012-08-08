#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <ros/ros.h>

#include <std_msgs/String.h>

#include <geometry_msgs/Twist.h>

#include <sstream>

#include <opencv2/opencv.hpp>

#include <QApplication>

#include <boost/thread.hpp>

#include "rosThread.h"
#include <brown_drivers/irobot_create_2_1/msg_gen/cpp/include/irobot_create_2_1/SensorPacket.h>
//#include "pclViwer.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    void sensorCB(const irobot_create_2_1::SensorPacket::ConstPtr& packet);
    void initView();
    ~MainWindow();
    
private slots:
    void on_openFileButton_clicked();
    void handleROSStartFailure();
    void handleRosThreadStart();

    void on_butBubbleProcessing_clicked();

private:
    Ui::MainWindow *ui;
    RosThread rosThread;
    ros::NodeHandle n;
 //   PclViwer* viv;
};

#endif // MAINWINDOW_H
