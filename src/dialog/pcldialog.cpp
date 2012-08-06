#include "pcldialog.h"
#include "ui_pcldialog.h"
#include "pcprocessing.h"
#include <QDebug>
#include <QVTKWidget.h>
#include <QFileDialog>
#include <QDir>
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

        path.append("/");

        PCprocessing::setDataSetPath(path);
        qDebug()<<"Dataset path is: "<<path;

        ui->lEditDSetPath->setText(path);

        QDir pathDir(path);

        QStringList list("*.pcd");

        QStringList tempList = pathDir.entryList(list/*, QDir::NoDotDot |QDir::NoDot,QDir::Name*/);

        QStringList sortedList;

        for(int i = 0; i < tempList.size(); i++){

            QString mm = ui->lEditCloudFileName->text();//"depth_";

            mm.append(QString::number(i+1));

            mm.append(".pcd");

            sortedList.push_back(mm);


        }

     //   qDebug()<<"Items"<<sortedList;

        if(tempList.size() != 0){

            PCprocessing::setDataSetItems(sortedList);

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

    if(num > 0 && num <= datasetSize){

         // pcl::PointCloud<pcl::PointXYZ> ss = temp.getCurrentCloud();

          temp.loadItem(num,temp.getCurrentCloud());

          ui->qvtkwidget->update();

    }

}

void PclDialog::on_butPrevItem_clicked()
{

    int num = ui->lEditItemNumber->text().toInt();

    num--;

    int datasetSize = PCprocessing::getNumofItems();

    if(num > 0 && num <= datasetSize){

          // pcl::PointCloud<pcl::PointXYZ> ss = temp.getCurrentCloud();

          temp.loadItem(num,temp.getCurrentCloud());

          ui->lEditItemNumber->setText(QString::number(num));

          ui->qvtkwidget->update();



    }

}

void PclDialog::on_butNextItem_clicked()
{
    int num = ui->lEditItemNumber->text().toInt();

    num++;

    int datasetSize = PCprocessing::getNumofItems();

    if(num > 0 && num <= datasetSize){


       // pcl::PointCloud<pcl::PointXYZ> ss = temp.getCurrentCloud();
          temp.loadItem(num,temp.getCurrentCloud());

          ui->lEditItemNumber->setText(QString::number(num));

          ui->qvtkwidget->update();



    }

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

}
