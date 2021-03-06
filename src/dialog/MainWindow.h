#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "pcprocessing.h"
#include "pcldialog.h"
#include "bubbleprocessdialog.h"
#include "bubbleprocess.h"
#include "imageprocess.h"
#include "imageprocessdialog.h"

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

#include"irobot.h"
#include "irobotdialog.h"
#include "databasemanager.h"

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

    void handleRobotDataSaveRequest();

    void on_butChooseDBFile_clicked();

    void on_butPlaceLabelsFile_clicked();

private:
    Ui::MainWindow *ui;
    RosThread* rosLoop;
    QThread* rosLoopThread;
    ros::NodeHandle n;
    PCprocessing pcProcess;

    //DatabaseManager* dbmanager;

    Irobot* robot;

    IRobotDialog* irobotDialog;
    PclDialog* pclDialog;
    ImageProcessDialog* impDialog;

    QString saveDataFileName;

 //   PclViwer* viv;
};

#endif // MAINWINDOW_H
