#include "imageprocessdialog.h"
#include "ui_imageprocessdialog.h"
#include "kinectutility.h"
#include "pcprocessing.h"
#include "imageprocess.h"
#include "bubbleprocess.h"

#include <QFileDialog>
#include <QCheckBox>
#include <QDebug>
#include <QListView>
#include <QAbstractItemModel>
#include <QStringListModel>
#include <QKeyEvent>

 QString filterpath ;

ImageProcessDialog::ImageProcessDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ImageProcessDialog)
{
    ui->setupUi(this);

    this->setWindowTitle("Image Process Dialog");

  //  ui->listViewBubbleNames->grabKeyboard();

  //  this->setAttribute(Qt::WA_DeleteOnClose);

   // QKeyEvent* event = new QKeyEvent(QEvent::KeyPress,Qt::Key_Delete,Qt::NoModifier);



}
ImageProcessDialog::ImageProcessDialog(QWidget *parent, PCprocessing *pcprocess):QDialog(parent),ui(new Ui::ImageProcessDialog)
{
    this->pcProcess = pcprocess;

    ui->setupUi(this);

    this->setWindowTitle("Image Process Dialog");

  //  ui->listViewBubbleNames->grabKeyboard();

    this->setAttribute(Qt::WA_DeleteOnClose);

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

        if(this->bubbleFileNames.size() > 0){

            this->bubbleFileNames.clear();

            QAbstractItemModel* model = new QStringListModel(this->bubbleFileNames);

            ui->listViewBubbleNames->setModel(model);

        }

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
    //QString fileName = "/home/hakan/Downloads/tekImge_CV2_1/filtreler/filtre";

    // Get the root directory
    filterpath =   QFileDialog::getOpenFileName(this,"Open Filter File","/home/hakan/Development", tr("*.txt"));

    if(filterpath != NULL)
    {
        //  QString filterType = "h";

        if(ui->cBoxTransposeFilter->checkState()  == Qt::Checked)
            ImageProcess::readFilter(filterpath,18,29,true,false,true);
        else
            ImageProcess::readFilter(filterpath,18,29,false,false,true);

        cv::destroyAllWindows();
    }
}

void ImageProcessDialog::on_butApplyAll_clicked()
{
    if(this->imageFiles.size() <= 0) return;

    if(this->filters.size() != this->invariantFileNames.size()) return;

    if(this->filters.size() != this->bubbleFileNames.size()) return;



    // Her bir filtre icin
    for(int i = 0; i < filters.size(); i++){

        ImageProcess::readFilter(filters.at(i),18,29,false,false,false);

        for(int j = 1; j <= this->imageFiles.size(); j++){

            QString tempPath = imageFiles.at(j-1);

            qDebug()<<"Temp path: "<<tempPath;

            Mat img = ImageProcess::loadImage(tempPath,false);

            // qDebug()<<img.channels();

            Mat ressg;

            cv::cvtColor(img,ressg,CV_BGR2GRAY);

            Mat sonuc = ImageProcess::applyFilter(ressg);

            //  vector<bubblePoint> resultt = bubbleProcess::convertGrayImage2Bub(channels[0],525,180);

            vector<bubblePoint> resultt = bubbleProcess::convertGrayImage2Bub(sonuc,525,255);

            QString saveBubbleName = ImageProcess::getDataSetPath();

            saveBubbleName.append(this->bubbleFileNames.at(i));

            QString str;

            str.setNum(j);

            saveBubbleName.append(str);

            saveBubbleName.append(".m");

            qDebug()<<saveBubbleName;

            vector<bubblePoint> resred ;

            QFile file(saveBubbleName);

            if(file.open(QFile::WriteOnly)){

                qDebug()<<resultt.size();

                resred = bubbleProcess::reduceBubble(resultt);

                qDebug()<<resred.size();

                bubbleProcess::saveBubble(&file,resred);

                file.close();

            }
            int noHarmonics = ui->lEditNoHarmonicsInvariant->text().toInt();

            bubbleProcess::calculateDFCoefficients(resred,ImageProcess::getDataSetPath(),"",j,noHarmonics,noHarmonics);

            bubbleProcess::calculateInvariants(resred,ImageProcess::getDataSetPath(),this->invariantFileNames.at(i),j,noHarmonics,noHarmonics);




        }

    }

   // int start = ui->lEditDatasetStart->text().toInt();

   // int end = ui->lEditDatasetEnd->text().toInt();




  /*  for(unsigned int i = 1; i <= fileNames.size(); i++)
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

        saveBubbleName.append(ui->lEditBubbleNameFilter->text());



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



*/
}

void ImageProcessDialog::on_cBoxTransposeFilter_stateChanged(int arg1)
{
    if(filterpath != NULL){

        if(arg1 == Qt::Checked){



            ImageProcess::readFilter(filterpath,18,29,true,false,true);

        }
        else
        {

            ImageProcess::readFilter(filterpath,18,29,false,false,true);

        }
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

void ImageProcessDialog::on_butGenerateInvariants_clicked()
{
    QString path = ImageProcess::getDataSetPath();

    if(path == NULL) return;



   // QString fileName = ui->lEditCloudFileName->text();

    QDir dirPath(path);

    dirPath.setFilter(QDir::NoDotAndDotDot | QDir::Files);

    int noHarmonics = ui->lEditNoHarmonicsInvariant->text().toInt();

    //dirPath.setNameFilters();

    QFileDialog dialog(this);
    dialog.setDirectory(dirPath);
    dialog.setFileMode(QFileDialog::ExistingFiles);

    QString filt =  ui->lEditBubbleNameFilter->text();
    filt.append("*");
    dialog.setNameFilter(filt);
    QStringList fileNames;

    if (dialog.exec())
        fileNames = dialog.selectedFiles();


    for(unsigned int i = 1; i <= fileNames.size(); i++)
    {

        QString tempPath = fileNames[i-1];

        QString outputFileName = ui->lEditInvariantName->text();

       // path.append(ui->lEditInvariantName->text());

        QFile file(tempPath);

        qDebug()<<"Bubble path is: "<<tempPath;

        if(file.open(QFile::ReadOnly))
        {

            vector<bubblePoint> bubble =  bubbleProcess::readBubble(&file);

            bubbleProcess::calculateDFCoefficients(bubble,ImageProcess::getDataSetPath(),"",i,noHarmonics,noHarmonics);

            bubbleProcess::calculateInvariants(bubble,ImageProcess::getDataSetPath(),outputFileName,i,noHarmonics,noHarmonics);

            file.close();
        }


    }
}

void ImageProcessDialog::on_butAddtoBubbleFileList_clicked()
{


    this->bubbleFileNames<<ui->lEditBubbleNameAdd->text();

    QAbstractItemModel* mod = new QStringListModel(bubbleFileNames);

    ui->listViewBubbleNames->setModel(mod);



    //ui->listViewBubbleNames->model()->insertRow(1);
    //ui->listViewBubbleNames->model()->insertColumn(1);

   // QVariant m = QVariant(ui->lEditBubbleNameAdd->text());

    //ui->listViewBubbleNames->model()->setData(ui->listViewBubbleNames->model()->index(0,0),m);



}

void ImageProcessDialog::on_butAddtoInputFileNames_clicked()
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
        this->imageFiles = dialog.selectedFiles();


    QAbstractItemModel* mod = new QStringListModel(this->imageFiles);

    ui->listViewInputFileNames->setModel(mod);



   // ui->listViewInputFileNames->setSelectionMode(QAbstractItemView::se)
}

void ImageProcessDialog::on_butAddtoFilterNames_clicked()
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
    dialog.setNameFilter("All filter files (*.txt)");

    if (dialog.exec())
        this->filters = dialog.selectedFiles();


    QAbstractItemModel* mod = new QStringListModel(this->filters);

    ui->listViewVisFilterNames->setModel(mod);

}

void ImageProcessDialog::on_butAddtoInvariantNames_clicked()
{


    this->invariantFileNames<<ui->lEditInvariantNameAdd->text();

    QAbstractItemModel* mod = new QStringListModel(this->invariantFileNames);

    ui->listViewInvariantNames->setModel(mod);

}

void ImageProcessDialog::on_butRemoveInputFileNames_clicked()
{

    if(ui->listViewInputFileNames->model())
    {
        ui->listViewInputFileNames->model()->removeRow(ui->listViewInputFileNames->currentIndex().row());

        QStringListModel* sss = (QStringListModel*)ui->listViewInputFileNames->model();

        this->imageFiles == sss->stringList();

        qDebug()<<this->imageFiles;

    }

}

void ImageProcessDialog::on_butRemoveFilterNames_clicked()
{

    if(ui->listViewVisFilterNames->model()){

        ui->listViewVisFilterNames->model()->removeRow(ui->listViewVisFilterNames->currentIndex().row());

        QStringListModel* sss = (QStringListModel*)ui->listViewVisFilterNames->model();

        this->filters == sss->stringList();

        qDebug()<<this->filters;

    }

}

void ImageProcessDialog::on_butRemoveBubbleFileNames_clicked()
{

    if(ui->listViewBubbleNames->model()){

        ui->listViewBubbleNames->model()->removeRow(ui->listViewBubbleNames->currentIndex().row());

        QStringListModel* sss = (QStringListModel*)ui->listViewBubbleNames->model();

        this->bubbleFileNames =  sss->stringList();

        qDebug()<<this->bubbleFileNames;
    }

}

void ImageProcessDialog::on_butRemoveInvariantFileNames_clicked()
{
    if(ui->listViewInvariantNames->model()){

         ui->listViewInvariantNames->model()->removeRow(ui->listViewInvariantNames->currentIndex().row());

        QStringListModel* sss = (QStringListModel*)ui->listViewInvariantNames->model();

         this->invariantFileNames =  sss->stringList();

        qDebug()<<this->invariantFileNames;
    }

}
