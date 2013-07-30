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
#include <ctime>
#include "databasemanager.h"


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

    initView();

}
ImageProcessDialog::ImageProcessDialog(QWidget *parent, PCprocessing *pcprocess):QDialog(parent),ui(new Ui::ImageProcessDialog)
{
    this->pcProcess = pcprocess;

    ui->setupUi(this);

    this->setWindowTitle("Image Process Dialog");

    //  ui->listViewBubbleNames->grabKeyboard();

    //  this->setAttribute(Qt::WA_DeleteOnClose);

    initView();
}
void ImageProcessDialog::initView()
{
    ui->lEditInputBubbleName->setText("bubble_hue_");

    ui->lEditInvariantName->setText("invariants_hue_");

    ui->lEditNoHarmonicsInvariant->setText("10");

    bubbleProcess::calculateImagePanAngles(525,640,480);

    bubbleProcess::calculateImageTiltAngles(525,640,480);

    ui->labelOrgImage->setScaledContents(true);

    ui->labelProcessedImage->setScaledContents(true);

    ui->lEditSatLower->setText(QString::number(ui->horsliderSatLower->value()));

    ui->lEditSatUpper->setText(QString::number(ui->horsliderSatUpper->value())) ;

    ui->lEditValLower->setText(QString::number(ui->horsliderValLower->value()));

    ui->lEditValUpper->setText(QString::number(ui->horsliderValUpper->value()));




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

        QImage* image = new QImage(path);

        if(image != NULL)
        {
            ui->labelOrgImage->setPixmap(QPixmap::fromImage(*image));

            Mat img = cv::imread(path.toStdString(),1);

            ImageProcess::setImage(img);

          /*  int satlower = ui->horsliderSatLower->value();

            int satupper = ui->horsliderSatUpper->value();

            int vallower = ui->horsliderValLower->value();

            int valupper = ui->horsliderValUpper->value();

            Mat hueChannel = ImageProcess::generateHueImage(satlower,satupper,vallower,valupper);

            QImage* image = new QImage(hueChannel.data,hueChannel.cols,hueChannel.rows,hueChannel.step,QImage::Format_Indexed8);

            ui->labelProcessedImage->setPixmap(QPixmap::fromImage(*image));*/
        }



      //  ui->l

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


    clock_t start,ends;

  /*  DatabaseManager* dbManager = new DatabaseManager(this);

    if(!dbManager->openDB(DB_PATH))
    {

        qDebug()<<"Database could not be opened!! returning...";

        return;
    }*/


    if(!DatabaseManager::openDB(DB_PATH))
    {

        qDebug()<<"Database could not be opened!! returning...";

        return;
    }

    // Her bir filtre icin
    for(int i = 0; i < filters.size(); i++)
    {

        int filterNumber = ImageProcess::getFrameNumber(filters.at(i));

        if(filterNumber == -1)
        {

            qDebug()<<"Error!! Filter number could not be determined, returning...";
            return;


        }

        ImageProcess::readFilter(filters.at(i),18,29,false,false,false);

        for(int j = 1; j <= this->imageFiles.size(); j++)
        {

            QString tempPath = imageFiles.at(j-1);

            qDebug()<<"Temp path: "<<tempPath;

            QString fullFilePath =tempPath;

            int frameNumber = ImageProcess::getFrameNumber(fullFilePath);

            if(frameNumber == -1){

                qDebug()<<"Error!! Frame number could not be determined, returning...";
                return;
            }


            start = clock();

            Mat img = ImageProcess::loadImage(tempPath,false);

            // qDebug()<<img.channels();

            Mat resg;

            cv::cvtColor(img,resg,CV_BGR2GRAY);

            Mat sonuc = ImageProcess::applyFilter(resg);

            //  vector<bubblePoint> resultt = bubbleProcess::convertGrayImage2Bub(channels[0],525,180);

            vector<bubblePoint> imgBubble = bubbleProcess::convertGrayImage2Bub(sonuc,525,255);

         /*   QString saveBubbleName = ImageProcess::getDataSetPath();

            saveBubbleName.append(this->bubbleFileNames.at(i));

            QString str;

            str.setNum(j);

            saveBubbleName.append(str);

            saveBubbleName.append(".m");

            qDebug()<<saveBubbleName;*/

            vector<bubblePoint> resred ;

            resred = bubbleProcess::reduceBubble(imgBubble);

            DatabaseManager::insertBubble(filterNumber,frameNumber,resred);

           /* QFile file(saveBubbleName);

            if(file.open(QFile::WriteOnly)){

                qDebug()<<resultt.size();

                resred = bubbleProcess::reduceBubble(resultt);

                 ends = clock();

                 qDebug()<<"Filter + bubble + generation time"<<((float)(ends-start)*1000/CLOCKS_PER_SEC);

                bubbleProcess::saveBubble(&file,resred);

                file.close();

            }*/
            int noHarmonics = ui->lEditNoHarmonicsInvariant->text().toInt();

            bubbleProcess::calculateDFCoefficients(resred,ImageProcess::getDataSetPath(),"",j,noHarmonics,noHarmonics);

            std::vector< std::vector< float > > invariants =  bubbleProcess::calculateInvariants(resred,ImageProcess::getDataSetPath(),this->invariantFileNames.at(i),j,noHarmonics,noHarmonics);

            //qDebug()<<resred.size()<<"invariants 0-0 "<<invariants[0][0];

            DatabaseManager::insertInvariants(filterNumber,frameNumber,invariants);


        } // END FOR

    } // END FOR

 //   dbManager->closeDB();

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


  //  QString fileName = ui->lEditCloudFileName->text();

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

    if(fileNames.size() == 0) return;

   // DatabaseManager* dbmanager= new DatabaseManager(this);

   /* if(!dbmanager->openDB(DB_PATH))
    {
        qDebug()<<"Bubble Database Could not be opened!! returning...";

        return;
    }*/

    if(!DatabaseManager::openDB(DB_PATH))
    {

        qDebug()<<"Bubble Database Could not be opened!! returning...";

        return;

    }
    for(unsigned int i = 1; i <= fileNames.size(); i++)
    {
        clock_t start,ends;

        QString fullPath = fileNames[i-1];

        qDebug()<<"Full Path: "<<fullPath;

        int frameNumber = ImageProcess::getFrameNumber(fullPath);

        if(frameNumber == -1)
        {

          //  dbmanager->closeDB();
            qDebug()<<"Error!! Frame number could not be determined, returning...";
            return;
        }

        Mat img = ImageProcess::loadImage(fullPath,false);

        if(img.empty())
        {
            qDebug()<<"Error!! Image could not be loaded, returning...";
        }

        int satLower =  ui->horsliderSatLower->value();

        int satUpper =  ui->horsliderSatUpper->value();

        int valLower =  ui->horsliderValLower->value();

        int valUpper = ui->horsliderValUpper->value();

        Mat hueChannel= ImageProcess::generateHueImage(img,satLower,satUpper,valLower,valUpper);

        QImage* image = new QImage(hueChannel.data,hueChannel.cols,hueChannel.rows,hueChannel.step,QImage::Format_Mono);

        ui->labelProcessedImage->setPixmap(QPixmap::fromImage(*image));

        start = clock();

        vector<bubblePoint> resultt = bubbleProcess::convertGrayImage2Bub(hueChannel,525,180);

        qDebug()<<resultt.size();

        vector<bubblePoint> resred = bubbleProcess::reduceBubble(resultt);

        ends = clock();

        qDebug()<<"Hue bubble generation time"<<((float)(ends-start)*1000/CLOCKS_PER_SEC)<<" ms";

        int noHarmonics = ui->lEditNoHarmonicsInvariant->text().toInt();

        DFCoefficients dfcoeff = bubbleProcess::calculateDFCoefficients(resred,noHarmonics,noHarmonics);

        std::vector< std::vector<float> > invariants = bubbleProcess::calculateInvariants(resred, dfcoeff,noHarmonics, noHarmonics);

        DatabaseManager::insertInvariants(HUE_TYPE,frameNumber,invariants);

     //   qDebug()<<resred.size();

       // DatabaseManager::insertBubble(HUE_TYPE,frameNumber,resred);

     /*   QString saveBubbleName = path;

        saveBubbleName.append("bubble_");

        saveBubbleName.append("hue_");

        QString str;

        str.setNum(i);

        saveBubbleName.append(str);

        saveBubbleName.append(".m");

        qDebug()<<saveBubbleName;


        QFile file(saveBubbleName);

        if(file.open(QFile::WriteOnly))
        {

            bubbleProcess::saveBubble(&file,resred);

            file.close();

        }

*/


    }

  //  dbmanager->closeDB();


}

void ImageProcessDialog::on_butGenerateInvariants_clicked()
{
   /* QString path = ImageProcess::getDataSetPath();

    if(path == NULL) return;

    // QString fileName = ui->lEditCloudFileName->text();

    QDir dirPath(path);

    dirPath.setFilter(QDir::NoDotAndDotDot | QDir::Files);*/

    int noHarmonics = ui->lEditNoHarmonicsInvariant->text().toInt();

    int bubbleType = ui->lEditInputBubbleType->text().toInt();

    int rangeMin = ui->lEditMinBubbleNum->text().toInt();

    int rangeMax = ui->lEditMaxBubbleNum->text().toInt();

    if(rangeMin >= rangeMax)
    {

        qDebug()<<"Invalid Range Values!! returning....";

           return;
}
  //  DatabaseManager* dbmanager = new DatabaseManager(this);

   /* if(!dbmanager->openDB(DB_PATH))
    {

        qDebug()<<"Failed to open database!! returning...";

        return;
    }*/

    if(!DatabaseManager::openDB(DB_PATH))
    {

        qDebug()<<"Failed to open database!! returning...";

        return;

    }

    for(int i = rangeMin; i < rangeMax; i++)
    {
        std::vector<bubblePoint>  bubble  = DatabaseManager::readBubble(bubbleType,i);

        if(bubble.size() > 0)
        {

            DFCoefficients dfcoeff = bubbleProcess::calculateDFCoefficients(bubble,noHarmonics,noHarmonics);

            std::vector< std::vector<float> > invariants = bubbleProcess::calculateInvariants(bubble, dfcoeff,noHarmonics, noHarmonics);

            DatabaseManager::insertInvariants(bubbleType,i,invariants);
        }
    }

   // dbmanager->closeDB();
    //int

    //dirPath.setNameFilters();

  /*  QFileDialog dialog(this);
    dialog.setDirectory(dirPath);
    dialog.setFileMode(QFileDialog::ExistingFiles);

    QString filt =  ui->lEditInputBubbleName->text();
    filt.append("*");
    dialog.setNameFilter(filt);
    QStringList fileNames;

    if (dialog.exec())
        fileNames = dialog.selectedFiles(); */


  /*  for(unsigned int i = 1; i <= fileNames.size(); i++)
    {

        QString tempPath = fileNames[i-1];

        QString outputFileName = ui->lEditInvariantName->text();

        // path.append(ui->lEditInvariantName->text());

        QFile file(tempPath);

        qDebug()<<"Bubble path is: "<<tempPath;
        clock_t start,ends;
        if(file.open(QFile::ReadOnly))
        {
            start = clock();
            vector<bubblePoint> bubble =  bubbleProcess::readBubble(&file);

            bubbleProcess::calculateDFCoefficients(bubble,ImageProcess::getDataSetPath(),"",i,noHarmonics,noHarmonics);

            bubbleProcess::calculateInvariants(bubble,ImageProcess::getDataSetPath(),outputFileName,i,noHarmonics,noHarmonics);

            ends = clock();

            qDebug()<<"Invariants + coefficients generation time"<<((float)(ends-start)*1000/CLOCKS_PER_SEC);

            file.close();
        }


    }*/
}

void ImageProcessDialog::on_butAddtoBubbleFileList_clicked()
{

   if(ui->lEditBubbleNameAdd->text().isEmpty()) return;

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

    if(ui->lEditInvariantNameAdd->text().isEmpty()) return;

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

        this->imageFiles.clear();

        this->imageFiles = sss->stringList();

        qDebug()<<this->imageFiles;

    }

}

void ImageProcessDialog::on_butRemoveFilterNames_clicked()
{

    if(ui->listViewVisFilterNames->model()){

        ui->listViewVisFilterNames->model()->removeRow(ui->listViewVisFilterNames->currentIndex().row());

        QStringListModel* sss = (QStringListModel*)ui->listViewVisFilterNames->model();

        this->filters.clear();

        this->filters = sss->stringList();

        qDebug()<<this->filters;

    }

}

void ImageProcessDialog::on_butRemoveBubbleFileNames_clicked()
{

    if(ui->listViewBubbleNames->model()){

        ui->listViewBubbleNames->model()->removeRow(ui->listViewBubbleNames->currentIndex().row());

        QStringListModel* sss = (QStringListModel*)ui->listViewBubbleNames->model();

        this->bubbleFileNames.clear();

        this->bubbleFileNames =  sss->stringList();

        qDebug()<<this->bubbleFileNames;
    }

}

void ImageProcessDialog::on_butRemoveInvariantFileNames_clicked()
{
    if(ui->listViewInvariantNames->model()){

        ui->listViewInvariantNames->model()->removeRow(ui->listViewInvariantNames->currentIndex().row());

        QStringListModel* sss = (QStringListModel*)ui->listViewInvariantNames->model();

        this->invariantFileNames.clear();

        this->invariantFileNames =  sss->stringList();

        qDebug()<<this->invariantFileNames;
    }

}

void ImageProcessDialog::on_butGenerateHueHistograms_clicked()
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

        cv::cvtColor(img,img,CV_BGR2HSV);

        std::vector<cv::Mat> channels ;

        cv::split(img,channels);

        long hsvHist[180];

        for(int j = 0; j< 180; j++){
            hsvHist[j] = 0;
        }

        for(int k = 0; k < img.rows; k++){
            for(int j = 0; j < img.cols; j++){

                // Saturation is between the range
                if(channels[2].at<uchar>(k,j) > 30 && channels[2].at<uchar>(k,j) < 225){

                    uchar val = channels[0].at<uchar>(k,j);
                    hsvHist[val] +=1;

                }


            }
        }
        QString fileName = path;

        QString number;

        number.setNum(i);

        fileName.append("huehist_");

        fileName.append(number);

        fileName.append(".txt");

        QFile output(fileName);

        if(output.open(QIODevice::WriteOnly))
        {

            qDebug()<<"Output path: "<<fileName;

            QTextStream stream(&output);

            for(int k = 0; k < 180; k++){

                stream<<hsvHist[k]<<"\n";
             }

            output.close();
        }

    }

}

void ImageProcessDialog::on_butGenerateHueHistBubble_clicked()
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
    dialog.setNameFilter("All hist files (*.txt)");
    QStringList fileNames;

    if (dialog.exec())
        fileNames = dialog.selectedFiles();

    for(unsigned int i = 1; i <= fileNames.size(); i++)
    {


        QString tempPath = fileNames[i-1];

        qDebug()<<"Temp path: "<<tempPath;

        QFile file(tempPath);

        if(file.open(QFile::ReadOnly))
        {
            QTextStream stream(&file);

            QString fileName = path;

            QString number;

            number.setNum(i);

            fileName.append("huehistbubble_");

            fileName.append(number);

            fileName.append(".m");

            QFile output(fileName);

            qDebug()<<"Output path: "<<fileName;

            if(output.open(QIODevice::WriteOnly))
            {

                QTextStream stream2(&output);

                int count = 0;

                while(1){

                    QString aLine = stream.readLine();

                    if(aLine == NULL) break;

                    double val = aLine.toDouble()/20000;

                    if(val >1) val = 1;

                    stream2<<count<<" "<<0<<" "<<val<<"\n";

                    count++;

                }

                output.close();

                file.close();
            }


        }



    }

       /* Mat img = ImageProcess::loadImage(tempPath,false);

        cv::cvtColor(img,img,CV_BGR2HSV);

        std::vector<cv::Mat> channels ;

        cv::split(img,channels);*/






}

void ImageProcessDialog::on_horsliderSatUpper_valueChanged(int value)
{
     ui->lEditSatUpper->setText(QString::number(value)) ;

    int satlower = ui->horsliderSatLower->value();

    int vallower = ui->horsliderValLower->value();

    int valupper =ui->horsliderValUpper->value();

    Mat hueChannel = ImageProcess::generateHueImage(satlower,value,vallower,valupper);

    QImage* image = new QImage(hueChannel.data,hueChannel.cols,hueChannel.rows,hueChannel.step,QImage::Format_Indexed8);

    ui->labelProcessedImage->setPixmap(QPixmap::fromImage(*image));

}

void ImageProcessDialog::on_horsliderSatLower_valueChanged(int value)
{

     ui->lEditSatLower->setText(QString::number(value));

    int satlower = value ;

    int satupper = ui->horsliderSatUpper->value();

    int vallower = ui->horsliderValLower->value();

    int valupper =ui->horsliderValUpper->value();

    Mat hueChannel = ImageProcess::generateHueImage(satlower,satupper,vallower,valupper);

    QImage* image = new QImage(hueChannel.data,hueChannel.cols,hueChannel.rows,hueChannel.step,QImage::Format_Indexed8);

    ui->labelProcessedImage->setPixmap(QPixmap::fromImage(*image));

}

void ImageProcessDialog::on_horsliderValUpper_valueChanged(int value)
{
    ui->lEditValUpper->setText(QString::number(value));

    int satlower = ui->horsliderSatLower->value();

    int satupper = ui->horsliderSatUpper->value();

    int vallower = ui->horsliderValLower->value();

    int valupper = value;

    Mat hueChannel = ImageProcess::generateHueImage(satlower,satupper,vallower,valupper);

    QImage* image = new QImage(hueChannel.data,hueChannel.cols,hueChannel.rows,hueChannel.step,QImage::Format_Indexed8);

    ui->labelProcessedImage->setPixmap(QPixmap::fromImage(*image));

}

void ImageProcessDialog::on_horsliderValLower_valueChanged(int value)
{

    ui->lEditValLower->setText(QString::number(value));

    int satlower = ui->horsliderSatLower->value();

    int satupper = ui->horsliderSatUpper->value();

    int vallower = value;

    int valupper =ui->horsliderValUpper->value();

    Mat hueChannel = ImageProcess::generateHueImage(satlower,satupper,vallower,valupper);

    QImage* image = new QImage(hueChannel.data,hueChannel.cols,hueChannel.rows,hueChannel.step,QImage::Format_Indexed8);

    ui->labelProcessedImage->setPixmap(QPixmap::fromImage(*image));

}

void ImageProcessDialog::on_butApplyFilter_clicked()
{










}
