#include "pcldialog.h"
#include "ui_pcldialog.h"
#include "pcprocessing.h"
#include <QDebug>
#include <QVTKWidget.h>
#include <QFileDialog>
#include <QDir>
#include <QtAlgorithms>

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
PCprocessing temp;

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
      viwer->addCoordinateSystem (1.0);
      viwer->initCameraParameters ();

      // Set the viewer of the point cloud processing class
      PCprocessing::setViewer(this->viwer);

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

        // BURAYA DIKKAT ET
        QStringList list("*.pcd");

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

          if(temp.loadItem(num,fileNam,temp.getCurrentCloud()))
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

    if ( temp.loadItem(num,fileNam,temp.getCurrentCloud())){

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
    if(temp.loadItem(num, fileNam, temp.getCurrentCloud())){

          ui->lEditItemNumber->setText(QString::number(num));

          ui->qvtkwidget->update();

    }

  //  }

}

void PclDialog::on_butVoxelGridFilter_clicked()
{

    temp.applyVoxelGridFilter(temp.getCurrentCloud());

     ui->qvtkwidget->update();


}

void PclDialog::on_butCalNormals_clicked()
{

    temp.calculateNormals(temp.getCurrentCloud());

    ui->qvtkwidget->update();

}

void PclDialog::on_butSaveNormalAngleHist_clicked()
{

    if(temp.saveNormalAngleHistogram(temp.getCurrentCloudNormals())){

        qDebug()<<"Normals successfully saved!!";
    }

}

void PclDialog::on_butRotateCloud_clicked()
{
    int rotX = ui->lEditRotationXDeg->text().toInt();

    int rotY = ui->lEditRotationYDeg->text().toInt();

    int rotZ = ui->lEditRotationZDeg->text().toInt();


   temp.rotatePointCloud(temp.getCurrentCloud(),rotX,rotY,rotZ);

    ui->qvtkwidget->update();

}

void PclDialog::on_butScalePointCloud_clicked()
{
    int scale = ui->lEditScalePointCloud->text().toInt();

    temp.scalePointCloud(temp.getCurrentCloud(),scale);

    ui->qvtkwidget->update();

}

void PclDialog::on_butSavePointCloud_clicked()
{

    int itemNumber = ui->lEditItemNumber->text().toInt();

    temp.savePointCloud(itemNumber);

}




void PclDialog::on_lEditCloudFileName_editingFinished()
{
    fileNam = ui->lEditCloudFileName->text();
}

void PclDialog::on_butApplyTransformationtoAll_clicked()
{
    for(int i = 0; i < 3; i++){

        ui->butNextItem->click();
        ui->butScalePointCloud->click();
        ui->butRotateCloud->click();
        ui->butSavePointCloud->click();

    }

}
