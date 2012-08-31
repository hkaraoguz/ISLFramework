#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "pcldialog.h"
#include "bubbleprocessdialog.h"
#include "bubbleprocess.h"
#include "imageprocess.h"

#include <QtCore/QFile>
#include <QtGui/QFileDialog>
#include <QtCore/QDebug>
#include <QtGui/QMessageBox>

//    // Message publisher for commanding velocity to our robot
ros::Publisher  createe;
ros::Subscriber subs;

//    // This is the required message type for velocity command
geometry_msgs::Twist twist;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    initView();

    rosThread.start();

     boost::shared_ptr<pcl::visualization::PCLVisualizer> viwer(new pcl::visualization::PCLVisualizer ("3D Viewer",false));

     PclDialog* newDialog = new PclDialog(this);

     newDialog->show();

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



}



void MainWindow::sensorCB(const irobot_create_2_1::SensorPacket::ConstPtr& packet){


    qDebug()<<packet->batteryCharge;

}

void MainWindow::on_openFileButton_clicked()
{
    QString fileName = "/home/hakan/Downloads/tekImge_CV2_1/filtreler/filtre";

    QString filterType = "v";

    ImageProcess::readFilter(fileName,18,29);

    for(int i = 1; i < 2668; i++){

        QString ss;

        ss.setNum(i);


        fileName = "/home/hakan/Development/ISL/Datasets/ImageClef2012/training1/std_cam/rgb_";

        fileName.append(ss);

        fileName.append(".jpg");

        qDebug()<<fileName;

        Mat ress = ImageProcess::loadImage(fileName);

        if(ress.rows != 0){

         //   Mat hsvMat;

        //    cvtColor(ress,hsvMat,CV_BGR2HSV);

         //   vector<Mat> channels;

        //   split(hsvMat,channels);

            Mat ressg;

            cv::cvtColor(ress,ressg,CV_BGR2GRAY);

           Mat sonuc = ImageProcess::applyFilter(ressg);

            destroyAllWindows();

          //  vector<bubblePoint> resultt = bubbleProcess::convertGrayImage2Bub(channels[0],525,180);

            vector<bubblePoint> resultt = bubbleProcess::convertGrayImage2Bub(sonuc,525,255);


            qDebug()<<resultt.size();

            vector<bubblePoint> resred = bubbleProcess::reduceBubble(resultt);

            qDebug()<<resred.size();


            fileName ="/home/hakan/Development/ISL/Datasets/ImageClef2012/training1/std_cam/bubble_";

            fileName.append(filterType); //filterType

            fileName.append("_");

            fileName.append(ss);

            fileName.append(".m");

            qDebug()<<fileName;

            QFile file(fileName);

            if(file.open(QFile::WriteOnly)){

                bubbleProcess::saveBubble(&file,resred);

                file.close();

            }

        }

}

   // createe.publish(twist);


 /*   QString path = QFileDialog::getOpenFileName(this,"Open list file","../");

    qDebug()<<path;

    QFile fil(path);

    QFile fil2("result.txt");

    fil2.open(QFile::WriteOnly|QFile::Text);

    QTextStream output(&fil2);

    fil.open(QFile::ReadOnly);

    QTextStream str(&fil);

    QString line = str.readLine();

    int count = 0;

    int titleCount = 0;

    QString url = NULL;
    QString title = NULL;

    bool urlOk = false;
    bool titleOk = false;

    while(!line.isNull()){


        QString temp = "File";

         if(line.contains(temp)){

              count++;

              temp.append(QString::number(count));

              temp.append("=");

             // qDebug()<<temp;

         }

         QString temp2 = "Title";

          if(line.contains(temp2)){

               titleCount++;

               temp2.append(QString::number(titleCount));

               temp2.append("=");

              // qDebug()<<temp2;

          }

          if(line.contains(temp))
          {
              line.remove(temp);

              line.remove("\n");

              url = line.trimmed();

              urlOk = true;



          }



        else if(line.contains(temp2))
        {
            line.remove(temp2);

             line.remove("\n");

            title = line.trimmed();

            if(title.length()< 15)
            titleOk = true;



        }

          if(urlOk && titleOk){

              output<<title<<" "<<url<<"\n";
              urlOk = false;
              titleOk = false;


          }

        line = str.readLine();


    }

    fil.close();

    fil2.close();

    QMessageBox::information(this,"Success","Channels has been parsed successfully");*/
}
void MainWindow::handleROSStartFailure(){

    QMessageBox::critical(this,"Attention","ROS has failed to start!!!");


}
void MainWindow::handleRosThreadStart(){



    //    ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);

     createe = n.advertise<geometry_msgs::Twist>("cmd_vel",1000);

     subs = n.subscribe("sensorPacket",1000,&MainWindow::sensorCB,this);


     qDebug()<<"Ros thread has started";


}

void MainWindow::on_butBubbleProcessing_clicked()
{
    BubbleProcessDialog* dlg = new BubbleProcessDialog(this);

    dlg->show();

}
