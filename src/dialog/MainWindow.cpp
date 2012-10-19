#include "MainWindow.h"
#include "ui_MainWindow.h"


#include <QtCore/QFile>
#include <QtGui/QFileDialog>
#include <QtCore/QDebug>
#include <QtGui/QMessageBox>

//    // Message publisher for commanding velocity to our robot
//ros::Publisher  createe;
//ros::Subscriber subs;

//    // This is the required message type for velocity command
geometry_msgs::Twist twist;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initView();

    rosThread.start();

  //   boost::shared_ptr<pcl::visualization::PCLVisualizer> viwer(new pcl::visualization::PCLVisualizer ("3D Viewer",false));


 //    viv = new PclViwer();

 //    viv->viewer = viwer;
    // viv->start();


     //    // Give a linear velocity of 0.1 m/s in x direction(forward)
     twist.linear.x = 0.1;


}

MainWindow::~MainWindow()
{
    this->rosThread.shutdownROS();
    while(rosThread.isRunning());

    //delete viv;
    delete ui;
}



void MainWindow::initView(){

    connect(&rosThread,SIGNAL(rosStartFailed(void)),this,SLOT(handleROSStartFailure(void)));

    connect(&rosThread,SIGNAL(rosStarted(void)),this,SLOT(handleRosThreadStart(void)));

   pclDialog = 0;
   irobotDialog = 0;
   robot = 0;


}



void MainWindow::sensorCB(const irobot_create_2_1::SensorPacket::ConstPtr& packet){


    qDebug()<<packet->batteryCharge;

}

void MainWindow::on_openFileButton_clicked()
{

   // ImageProcessDialog* dlg = new ImageProcessDialog(this,&pcProcess);

    //dlg->setPCprocessing(&this->pcProcess);

  //  dlg->show();


    if (!pclDialog){
        pclDialog = new PclDialog(this,&pcProcess);

        pcProcess.setViewer(pclDialog->viwer);
        pcProcess.initializeViewer();

        pclDialog->show();
        pclDialog->raise();
        pclDialog->activateWindow();




    }
    else{

        pclDialog->show();
        pclDialog->raise();
        pclDialog->activateWindow();
    }

}
void MainWindow::handleROSStartFailure(){

    QMessageBox::critical(this,"Attention","ROS has failed to start!!!");


}
void MainWindow::handleRosThreadStart(){



    //    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);

  //   createe = n.advertise<geometry_msgs::Twist>("cmd_vel",1000);

  //   subs = n.subscribe("sensorPacket",1000,&MainWindow::sensorCB,this);

    robot = new Irobot(this);

    if(robot->initIrobotConnection()){

        if(!irobotDialog){

            irobotDialog = new IRobotDialog(this,robot);
        }

        irobotDialog->show();


    }


     qDebug()<<"Ros thread has started";


}

void MainWindow::on_butBubbleProcessing_clicked()
{
     pcProcess.initializeViewer();



  //  BubbleProcessDialog* dlg = new BubbleProcessDialog(this);

 //   dlg->show();

}
