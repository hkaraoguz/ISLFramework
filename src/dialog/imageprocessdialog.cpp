#include "imageprocessdialog.h"
#include "ui_imageprocessdialog.h"
#include "kinectutility.h"
#include "pcprocessing.h"
#include "imageprocess.h"
#include "bubbleprocess.h"

#include <QFileDialog>
#include <QCheckBox>
#include <QDebug>


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

void ImageProcessDialog::on_butSetDSetPath_clicked()
{
    QString path = PCprocessing::getDataSetPath();

    if(path != NULL){

        ImageProcess::setDataSetPath(path);

        ui->lEditDSetPath->setText(path);

        return;

    }

    path = QFileDialog::getExistingDirectory(this,"Set dataset path","/home/hakan",QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(path != NULL)
    {
        path.append("/");

        ImageProcess::setDataSetPath(path);

        ui->lEditDSetPath->setText(path);

    }

}

void ImageProcessDialog::on_butLoadFilter_clicked()
{
    QString fileName = "/home/hakan/Downloads/tekImge_CV2_1/filtreler/filtre";

    QString filterType = "h";

    if(ui->cBoxTransposeFilter->checkState()  == Qt::Checked)
        ImageProcess::readFilter(fileName,18,29,true,false,true);
    else
        ImageProcess::readFilter(fileName,18,29,false,false,true);

    cv::destroyAllWindows();

}

void ImageProcessDialog::on_butApplyAll_clicked()
{

   // int start = ui->lEditDatasetStart->text().toInt();

   // int end = ui->lEditDatasetEnd->text().toInt();

    QString path = ImageProcess::getDataSetPath();

    if(path == NULL) return;


    QString fileName = ui->lEditCloudFileName->text();

    QDir dirPath(path);

    dirPath.setFilter(QDir::NoDotAndDotDot | QDir::Files);

    //dirPath.setNameFilters();

    QFileDialog dialog(this);
    dialog.setDirectory(dirPath);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter("All image files (*.png *.jpeg *.jpg)");
    QStringList fileNames;

    if (dialog.exec())
        fileNames = dialog.selectedFiles();


    for(unsigned int i = 1; i <= fileNames.size(); i++)
    {

        QString tempPath = fileNames[i-1];

        qDebug()<<"Temp path: "<<tempPath;

        Mat img = ImageProcess::loadImage(tempPath,false);

        // qDebug()<<img.channels();

        Mat ressg;


        cv::cvtColor(img,ressg,CV_BGR2GRAY);

        Mat sonuc = ImageProcess::applyFilter(ressg);



        //  vector<bubblePoint> resultt = bubbleProcess::convertGrayImage2Bub(channels[0],525,180);

        vector<bubblePoint> resultt = bubbleProcess::convertGrayImage2Bub(sonuc,525,255);


        qDebug()<<resultt.size();

        vector<bubblePoint> resred = bubbleProcess::reduceBubble(resultt);

        qDebug()<<resred.size();

        QString saveBubbleName = path;

        saveBubbleName.append("bubble_");

        if(ui->cBoxTransposeFilter->checkState() == Qt::Checked){

            saveBubbleName.append("v_");

        }
        else
            saveBubbleName.append("h_");

        QString str;

        str.setNum(i);

        saveBubbleName.append(str);

        saveBubbleName.append(".m");

        qDebug()<<saveBubbleName;


        QFile file(saveBubbleName);

        if(file.open(QFile::WriteOnly)){

            bubbleProcess::saveBubble(&file,resred);

            file.close();

        }







    }

    /*   for(int i = 1; i < 2533; i++){

           QString ss;

           ss.setNum(i);


           fileName = "/home/hakan/Development/ISL/Datasets/ImageClef2012/training2/std_cam/rgb_";

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

          //     destroyAllWindows();

             //  vector<bubblePoint> resultt = bubbleProcess::convertGrayImage2Bub(channels[0],525,180);

               vector<bubblePoint> resultt = bubbleProcess::convertGrayImage2Bub(sonuc,525,255);


               qDebug()<<resultt.size();

               vector<bubblePoint> resred = bubbleProcess::reduceBubble(resultt);

               qDebug()<<resred.size();


               fileName ="/home/hakan/Development/ISL/Datasets/ImageClef2012/training2/std_cam/bubble_";

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
*/

}

void ImageProcessDialog::on_cBoxTransposeFilter_stateChanged(int arg1)
{
    QString fileName = "/home/hakan/Downloads/tekImge_CV2_1/filtreler/filtre";

    if(arg1 == Qt::Checked){



        ImageProcess::readFilter(fileName,18,29,true,false,true);

    }
    else{

        ImageProcess::readFilter(fileName,18,29,false,false,true);

    }
}

void ImageProcessDialog::on_butGenerateHueBubble_clicked()
{

    QString path = ImageProcess::getDataSetPath();

    if(path == NULL) return;


    QString fileName = ui->lEditCloudFileName->text();

    QDir dirPath(path);

    dirPath.setFilter(QDir::NoDotAndDotDot | QDir::Files);

    //dirPath.setNameFilters();

    QFileDialog dialog(this);
    dialog.setDirectory(dirPath);
    dialog.setFileMode(QFileDialog::ExistingFiles);
    dialog.setNameFilter("All image files (*.png *.jpeg *.jpg)");
    QStringList fileNames;

    if (dialog.exec())
        fileNames = dialog.selectedFiles();


    for(unsigned int i = 1; i <= fileNames.size(); i++)
    {

        QString tempPath = fileNames[i-1];

        qDebug()<<"Temp path: "<<tempPath;

        Mat img = ImageProcess::loadImage(tempPath,false);

        // qDebug()<<img.channels();

        Mat hsvimg;


        cv::cvtColor(img,hsvimg,CV_BGR2HSV);

        std::vector<Mat> channels;

        cv::split(hsvimg,channels);

       /* cvNamedWindow("hue");

        imshow("hue",channels[0]);

        waitKey();

        destroyWindow("hue");*/

        vector<bubblePoint> resultt = bubbleProcess::convertGrayImage2Bub(channels[0],525,180);



        qDebug()<<resultt.size();

        vector<bubblePoint> resred = bubbleProcess::reduceBubble(resultt);

        qDebug()<<resred.size();

        QString saveBubbleName = path;

        saveBubbleName.append("bubble_");

        saveBubbleName.append("hue_");

        QString str;

        str.setNum(i);

        saveBubbleName.append(str);

        saveBubbleName.append(".m");

        qDebug()<<saveBubbleName;


        QFile file(saveBubbleName);

        if(file.open(QFile::WriteOnly)){

            bubbleProcess::saveBubble(&file,resred);

            file.close();

        }







    }


}
