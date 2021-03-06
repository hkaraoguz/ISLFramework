#include "MainWindow.h"
#include "ui_MainWindow.h"


#include <QtCore/QFile>
#include <QtGui/QFileDialog>
#include <QtCore/QDebug>
#include <QtGui/QMessageBox>
#include <QtCore/QTime>
#include <QtCore/QDateTime>

#include <ros/time.h>

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



    rosLoop =  new RosThread;
    rosLoopThread = new QThread(this);


    rosLoop->moveToThread(rosLoopThread);

    connect(rosLoopThread,SIGNAL(finished()),rosLoop,SLOT(deleteLater()));
    connect(rosLoopThread,SIGNAL(started()),rosLoop,SLOT(loop()));



     initView();

     rosLoopThread->start();







  //  dbmanager = new DatabaseManager(this);

   /* QString path(QDir::home().path());

    path.append("/Development/ISL/Datasets/ImageClef2012/bubble.db");

    if(DatabaseManager::openDB(path))
    {

        qDebug()<<"DB successfully opened";

       DatabaseManager::insertRowtoBubble(0,1,110,20,0.136);

       /* dbmanager->insertRowtoBubble(0,1,111,22,0.84);

        dbmanager->insertRowtoBubble(0,1,111,22,0.80);

        dbmanager->insertRowtoBubble(0,2,111,22,0.80);

      //  dbmanager->readBubble(0,1);
    }*/

  //   boost::shared_ptr<pcl::visualization::PCLVisualizer> viwer(new pcl::visualization::PCLVisualizer ("3D Viewer",false));


 //    viv = new PclViwer();

 //    viv->viewer = viwer;
    // viv->start();


     //    // Give a linear velocity of 0.1 m/s in x direction(forward)
  //   twist.linear.x = 0.1;


}

MainWindow::~MainWindow()
{
    this->rosLoop->shutdownROS();
    rosLoopThread->quit();
    while(rosLoopThread->isRunning());
    DatabaseManager::closeDB();
    //delete viv;
    delete ui;
}



void MainWindow::initView(){

    connect(rosLoop,SIGNAL(rosStartFailed(void)),this,SLOT(handleROSStartFailure(void)));

    connect(rosLoop,SIGNAL(rosStarted(void)),this,SLOT(handleRosThreadStart(void)));

   pclDialog = 0;
   irobotDialog = 0;
   impDialog = 0;
   robot = 0;

   ui->lEditImageWidth->setText("640");
   ui->lEditImageHeight->setText("480");

   ui->lEditFocalLength->setText("525");

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
        pcProcess.initializeViewer(true);

        pclDialog->show();
        pclDialog->raise();
        pclDialog->activateWindow();

        connect(pclDialog,SIGNAL(kinectSave()),this,SLOT(handleRobotDataSaveRequest()));


    }
    else{

        pclDialog->show();
        pclDialog->raise();
        pclDialog->activateWindow();
    }

    if(!impDialog){

        int imageWidth = ui->lEditImageWidth->text().toInt();

        int imageHeight = ui->lEditImageHeight->text().toInt();

        int focalLength = ui->lEditFocalLength->text().toInt();


        impDialog = new ImageProcessDialog(this,imageWidth,imageHeight,focalLength);

        impDialog->show();
        impDialog->raise();
        impDialog->activateWindow();

    }
    else
    {

        impDialog->show();
        impDialog->raise();
        impDialog->activateWindow();


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

    robot->rosThread = this->rosLoop;

   // qRegisterMetaType<QVector<double> >("QVector<double>");
   // connect(robot,SIGNAL(velocityCommand(QVector<double>)),rosLoop,SLOT(handleVelocityCommand(QVector<double>)));

    if(!irobotDialog){

        irobotDialog = new IRobotDialog(this,robot);
    }

    irobotDialog->show();

  /*  if(robot->initIrobotConnection()){

        if(!irobotDialog){

            irobotDialog = new IRobotDialog(this,robot);
        }

        irobotDialog->show();


    }*/


     qDebug()<<"Ros thread has started";


}

void MainWindow::on_butBubbleProcessing_clicked()
{
   //  pcProcess.initializeViewer();



    BubbleProcessDialog* dlg = new BubbleProcessDialog(this);

    dlg->show();

}

void MainWindow::handleRobotDataSaveRequest()
{
    if(!robot->isConnected()) return;

    if(saveDataFileName == NULL){

        QDateTime noww = QDateTime::currentDateTime();

        saveDataFileName = "robotData_";

        saveDataFileName.append(noww.toString());

        saveDataFileName.append(".txt");

        QFile file(saveDataFileName);


        if(file.open(QFile::Append)){

            //   QTime noww = QTime::currentTime();

            //   QTextStream str(&file);

            //   str<<noww.toString()<<"\n";


            this->robot->saveData(&file);

            file.close();


        }


    }
    else{

        QFile file(saveDataFileName);


        if(file.open(QFile::Append)){

            //   QTime noww = QTime::currentTime();

            //   QTextStream str(&file);

            //   str<<noww.toString()<<"\n";


            this->robot->saveData(&file);

            file.close();


        }


    }

}

void MainWindow::on_butChooseDBFile_clicked()
{
    // Get the root directory
    QString path =   QFileDialog::getOpenFileName(this,"Open DB File","/home/hakan/Development/ISL/Datasets",tr("*.db"));

    if(path != NULL)
    {
        ui->lEditDBFilePath->setText(path);

        if(DatabaseManager::openDB(path))
        {
            qDebug()<<"Database opened Successfully!!";
        }
    }
}

void MainWindow::on_butPlaceLabelsFile_clicked()
{
    // Get the root directory
    QString path =   QFileDialog::getOpenFileName(this,"Open Place Labels File","/home/hakan/Development/ISL/Datasets",tr("*.txt"));

    if(path != NULL)
    {
        ui->lEditPlaceLabelsFilePath->setText(path);

        DatabaseManager::determinePlaceLabels(path);

        //ui->lEditDBFilePath
    }

}
