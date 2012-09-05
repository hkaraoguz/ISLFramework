#include "pcldialog.h"
#include "ui_pcldialog.h"
#include "pcprocessing.h"
#include "bubbleprocess.h"
#include <QDebug>
#include <QVTKWidget.h>
#include <QFileDialog>
#include <QDir>
#include <QtAlgorithms>
#include <QThread>

QString fileNam;

bool compareNames(const QString& s1,const QString& s2)
{
    //apply section(),mid() or whatever to take out the integer part and compare

    //for example if all the strings are like "userXXX.jpg"
    QString temp1=s1.section(fileNam,1);//temp1=="XXX.jpg";
    temp1=temp1.section('.',0,0);//temp1=="XXX"

    QString temp2=s2.section(fileNam,1);
    temp2=temp2.section('.',0,0);

    return (temp1.toInt()<temp2.toInt());

    //your code would be more complicated than this as u have different file names
    //and filenames may also contain periods(.) more than once


}
void PclDialog::setPCprocessing(PCprocessing *pcprocess){

    this->pcProcessing = pcprocess;

}

PclDialog::PclDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PclDialog)
{
    ui->setupUi(this);

    // This is the visualizer that is embedded inside qVTKWidget
    viwer = boost::shared_ptr<pcl::visualization::PCLVisualizer>(new pcl::visualization::PCLVisualizer ("3D Viewer",false));

    vtkSmartPointer<vtkRenderWindow> renderWindow = viwer->getRenderWindow();

    ui->qvtkwidget->SetRenderWindow(renderWindow);

    viwer->setBackgroundColor (0, 0, 0);

    viwer->initCameraParameters ();

    // Set the viewer of the point cloud processing class
    PCprocessing::setViewer(this->viwer);

    sleep(1);

    this->initializeView();


}

PclDialog::~PclDialog()
{
    delete ui;
}
void PclDialog::initializeView(){

    ui->lEditCloudFileName->setText("depth_");

    ui->lEditRotationXDeg->setText("0");

    ui->lEditRotationYDeg->setText("0");

    ui->lEditRotationZDeg->setText("0");

    ui->lEditScalePointCloud->setText("1");

    ui->lEditDatasetStart->setText("1");

    ui->lEditDatasetEnd->setText("2");

    ui->lEditCloudMaxRange->setText("6");

    ui->lEditInputBubbleName->setText("bubble_");

    ui->lEditOutputInvName->setText("invariants_");


     viwer->addCoordinateSystem (1.0);

}

void PclDialog::on_butSetDSetPath_clicked()
{
    QString path = QFileDialog::getExistingDirectory(this,"Set dataset path","/home/hakan",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(path != NULL){

        fileNam = ui->lEditCloudFileName->text();

        path.append("/");

        PCprocessing::setDataSetPath(path);
        qDebug()<<"Dataset path is: "<<path;

        ui->lEditDSetPath->setText(path);

        QDir pathDir(path);

        QString ss = fileNam;

        ss.append("*.pcd");

        // BURAYA DIKKAT ET
        QStringList list(ss);

        QStringList tempList = pathDir.entryList(list/*, QDir::NoDotDot |QDir::NoDot,QDir::Name*/);

        qSort(tempList.begin(),tempList.end(),compareNames);

        QStringList sortedList;

        /*  for(int i = 0; i < tempList.size(); i++){

            QString mm = ui->lEditCloudFileName->text();//"depth_";

            mm.append(QString::number(i));

            mm.append(".pcd");

            sortedList.push_back(mm);


        }*/

        qDebug()<<"Items"<<tempList;

        if(tempList.size() != 0){

            PCprocessing::setDataSetItems(tempList);

            ui->lEditItemNumber->setText(QString::number(0));


        }



        //  PCprocessing temp;

        //   temp.loadItem(181);


    }
}

void PclDialog::on_butLoadItem_clicked()
{
    int num = ui->lEditItemNumber->text().toInt();

    int datasetSize = PCprocessing::getNumofItems();

    //  if(num >= 0 && num < datasetSize){

    // pcl::PointCloud<pcl::PointXYZ> ss = temp.getCurrentCloud();

    if(pcProcessing->loadItem(num,fileNam,pcProcessing->getCurrentCloud()))
        ui->qvtkwidget->update();

    //   }

}

void PclDialog::on_butPrevItem_clicked()
{

    int num = ui->lEditItemNumber->text().toInt();

    num--;

    int datasetSize = PCprocessing::getNumofItems();

    // if(num >= 0 && num < datasetSize){

    // pcl::PointCloud<pcl::PointXYZ> ss = temp.getCurrentCloud();

    if ( pcProcessing->loadItem(num,fileNam,pcProcessing->getCurrentCloud())){

        ui->lEditItemNumber->setText(QString::number(num));

        ui->qvtkwidget->update();

    }

    //  }

}

void PclDialog::on_butNextItem_clicked()
{
    int num = ui->lEditItemNumber->text().toInt();

    num++;

    //  int datasetSize = PCprocessing::getNumofItems();

    //   if(num > 0 && num < datasetSize){


    // pcl::PointCloud<pcl::PointXYZ> ss = temp.getCurrentCloud();
    if(pcProcessing->loadItem(num, fileNam, pcProcessing->getCurrentCloud())){

        ui->lEditItemNumber->setText(QString::number(num));

        ui->qvtkwidget->update();

    }

    //  }

}

void PclDialog::on_butVoxelGridFilter_clicked()
{

    pcProcessing->applyVoxelGridFilter(pcProcessing->getCurrentCloud());

    ui->qvtkwidget->update();


}

void PclDialog::on_butCalNormals_clicked()
{

    pcProcessing->calculateNormals(pcProcessing->getCurrentCloud());

    ui->qvtkwidget->update();

}

void PclDialog::on_butSaveNormalAngleHist_clicked()
{
     int itemNumber = ui->lEditItemNumber->text().toInt();

    if(pcProcessing->saveNormalAngleHistogram(pcProcessing->getCurrentCloudNormals(),itemNumber)){

        qDebug()<<"Normals successfully saved!!";
    }

}

void PclDialog::on_butRotateCloud_clicked()
{
    int rotX = ui->lEditRotationXDeg->text().toInt();

    int rotY = ui->lEditRotationYDeg->text().toInt();

    int rotZ = ui->lEditRotationZDeg->text().toInt();


    pcProcessing->rotatePointCloud(pcProcessing->getCurrentCloud(),rotX,rotY,rotZ);

    ui->qvtkwidget->update();

}

void PclDialog::on_butScalePointCloud_clicked()
{
    int scale = ui->lEditScalePointCloud->text().toInt();

    pcProcessing->scalePointCloud(pcProcessing->getCurrentCloud(),scale);

    ui->qvtkwidget->update();

}

void PclDialog::on_butSavePointCloud_clicked()
{

    int itemNumber = ui->lEditItemNumber->text().toInt();

    pcProcessing->savePointCloud(itemNumber,fileNam);

}




void PclDialog::on_lEditCloudFileName_editingFinished()
{
    fileNam = ui->lEditCloudFileName->text();
}

void PclDialog::on_butApplyTransformationtoAll_clicked()
{
    int size = pcProcessing->getNumofItems();

    int start = ui->lEditDatasetStart->text().toInt();

    int endd = ui->lEditDatasetEnd->text().toInt();

    for(int i = start; i < endd; i++){

        if(pcProcessing->loadItem(i, fileNam, pcProcessing->getCurrentCloud()))
        {

            ui->butScalePointCloud->click();
            ui->butRotateCloud->click();

            pcProcessing->savePointCloud(i,fileNam);
        }


        //  ui->butSavePointCloud->click();

        //  ui->butNextItem->click();

    }

}

void PclDialog::on_butGeneratePointCloudBubble_clicked()
{
    std::vector<bubblePointXYZ> bubble;

    sensor_msgs::PointCloud2::Ptr cloud = pcProcessing->getCurrentCloud();

    pcl::PointCloud<pcl::PointXYZRGB> normalCloud;

    pcl::fromROSMsg(*cloud,normalCloud);

    for(unsigned int i = 0; i < normalCloud.points.size(); i++){

        bubblePointXYZ pt;

        pt.x = normalCloud.points.at(i).x;
        pt.y = normalCloud.points.at(i).y;
        pt.z = normalCloud.points.at(i).z;

        bubble.push_back(pt);

    }

    double maxRangeMeters = ui->lEditCloudMaxRange->text().toDouble();

    vector<bubblePoint> sphBubble = bubbleProcess::convertBubXYZ2BubSpherical(bubble,maxRangeMeters);

    vector<bubblePoint> sphRedBubble = bubbleProcess::reduceBubble(sphBubble);

    QString pathh = pcProcessing->getDataSetPath();



    pathh.append("bubble_");
    pathh.append(ui->lEditItemNumber->text());
    pathh.append(".m");

    QFile file(pathh);

    if(file.open(QFile::WriteOnly))
    {

        bubbleProcess::saveBubble(&file,sphRedBubble);

        file.close();

    }

    int itemNo = ui->lEditItemNumber->text().toInt();

    qDebug()<<"Item no: "<<itemNo<<"\n";

    bubbleProcess::calculateDFCoefficients(sphRedBubble,pcProcessing->getDataSetPath(),"",itemNo,30,30);


  //  QString inputBubbleName = ui->lEditInputBubbleName->text();

    QString outputFileName =  ui->lEditOutputInvName->text();

    bubbleProcess::calculateInvariants(sphRedBubble,pcProcessing->getDataSetPath(),outputFileName,itemNo,30,30);

}

void PclDialog::on_butGeneratePointCloudBubbles_clicked()
{
    //  double maxRangeMeters = ui->lEditCloudMaxRange->text().toDouble();


    int start = ui->lEditDatasetStart->text().toInt();

    int endd = ui->lEditDatasetEnd->text().toInt();

    for(int i = start; i < endd; i++){

        if(pcProcessing->loadItem(i, fileNam, pcProcessing->getCurrentCloud()))
        {



            std::vector<bubblePointXYZ> bubble;

            sensor_msgs::PointCloud2::Ptr cloud = pcProcessing->getCurrentCloud();

            pcl::PointCloud<pcl::PointXYZRGB> normalCloud;

            pcl::fromROSMsg(*cloud,normalCloud);

            for(unsigned int i = 0; i < normalCloud.points.size(); i++){

                bubblePointXYZ pt;

                pt.x = normalCloud.points.at(i).x;
                pt.y = normalCloud.points.at(i).y;
                pt.z = normalCloud.points.at(i).z;

                bubble.push_back(pt);

            }

            double maxRangeMeters = ui->lEditCloudMaxRange->text().toDouble();

            vector<bubblePoint> sphBubble = bubbleProcess::convertBubXYZ2BubSpherical(bubble,maxRangeMeters);

            vector<bubblePoint> sphRedBubble = bubbleProcess::reduceBubble(sphBubble);

            QString pathh = pcProcessing->getDataSetPath();

            pathh.append("bubble_");

            QString ss;

            ss.setNum(i);

            pathh.append(ss);
            pathh.append(".m");

            QFile file(pathh);

            qDebug()<<"Bubble path is: "<<pathh;

            if(file.open(QFile::WriteOnly))
            {

                bubbleProcess::saveBubble(&file,sphRedBubble);

                file.close();

            }

        }
    }
}

void PclDialog::on_butCalculateBubbleInvariants_clicked()
{
    int start = ui->lEditDatasetStart->text().toInt();

    int endd = ui->lEditDatasetEnd->text().toInt();

    QString inputBubbleName = ui->lEditInputBubbleName->text();

    QString outputFileName =  ui->lEditOutputInvName->text();

    for(int i = start; i < endd; i++){

        QString pathh = pcProcessing->getDataSetPath();

        pathh.append(inputBubbleName);

        QString ss;

        ss.setNum(i);

        pathh.append(ss);
        pathh.append(".m");

        QFile file(pathh);

        qDebug()<<"Bubble path is: "<<pathh;

        if(file.open(QFile::ReadOnly))
        {

            vector<bubblePoint> bubble =  bubbleProcess::readBubble(&file);

            bubbleProcess::calculateDFCoefficients(bubble,pcProcessing->getDataSetPath(),"",i,10,10);

            bubbleProcess::calculateInvariants(bubble,pcProcessing->getDataSetPath(),outputFileName,i,10,10);

            file.close();
        }


    }


}

void PclDialog::on_butCalculateAllNormalAngleHistogram_clicked()
{
    int start = ui->lEditDatasetStart->text().toInt();

    int endd = ui->lEditDatasetEnd->text().toInt();

   // QString inputBubbleName = ui->lEditInputBubbleName->text();

   // QString outputFileName =  ui->lEditOutputInvName->text();

    for(int i = start; i < endd; i++){

        if(pcProcessing->loadItem(i, fileNam, pcProcessing->getCurrentCloud()))
        {

            pcProcessing->applyVoxelGridFilter(pcProcessing->getCurrentCloud());

            pcProcessing->calculateNormals(pcProcessing->getCurrentCloud());

            if(pcProcessing->saveNormalAngleHistogram(pcProcessing->getCurrentCloudNormals(),i)){

                qDebug()<<"Normals successfully saved!!";
            }
        }
    }
}
