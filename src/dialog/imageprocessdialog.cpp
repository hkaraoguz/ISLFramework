#include "imageprocessdialog.h"
#include "ui_imageprocessdialog.h"
#include "kinectutility.h"
#include "pcprocessing.h"
#include "imageprocess.h"

#include <QFileDialog>


ImageProcessDialog::ImageProcessDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageProcessDialog)
{
    ui->setupUi(this);
}

ImageProcessDialog::~ImageProcessDialog()
{
    delete ui;
}
void ImageProcessDialog::setPCprocessing(PCprocessing *pcprocess){

    this->pcProcess = pcprocess;

}

void ImageProcessDialog::on_but_LoadImage_clicked()
{
    // Get the root directory
    QString path =   QFileDialog::getOpenFileName(this,"Open Image File","/home/hakan/Development",NULL);

    if(path != NULL)
    {

        //char pathChar[300];



        QByteArray ba = path.toLocal8Bit();
        char* pathChar = ba.data();

        KinectUtility::convertDepthText2Cloud(pathChar,pcProcess->getCurrentCloud());


       // Mat img = ImageProcess::loadImage(path);

       // KinectUtility::convertDepthImage2Cloud(img,pcProcess->getCurrentCloud());

        pcProcess->showPointCloud(pcProcess->getCurrentCloud());


        Mat sss;

    }

}
