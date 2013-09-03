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
ImageProcessDialog::ImageProcessDialog(QWidget *parent, int imageWidth, int imageHeight, int focalLengthPixels):QDialog(parent),ui(new Ui::ImageProcessDialog)
{

    ui->setupUi(this);

    this->setWindowTitle("Image Process Dialog");

    //  ui->listViewBubbleNames->grabKeyboard();

    //  this->setAttribute(Qt::WA_DeleteOnClose);


    this->focalLengthPixels = focalLengthPixels;

    this->imageHeight = imageHeight;

    this->imageWidth = imageWidth;

    initView();




}
void ImageProcessDialog::initView()
{
  //  ui->lEditInputBubbleName->setText("bubble_hue_");

   // ui->lEditInvariantName->setText("invariants_hue_");

    ui->lEditNoHarmonicsInvariant->setText("10");

    bubbleProcess::calculateImagePanAngles(focalLengthPixels,imageWidth,imageHeight);

    bubbleProcess::calculateImageTiltAngles(focalLengthPixels,imageWidth,imageHeight);

    ui->labelOrgImage->setScaledContents(true);

    ui->labelProcessedImage->setScaledContents(true);

    ui->lEditSatLower->setText(QString::number(ui->horsliderSatLower->value()));

    ui->lEditSatUpper->setText(QString::number(ui->horsliderSatUpper->value())) ;

    ui->lEditValLower->setText(QString::number(ui->horsliderValLower->value()));

    ui->lEditValUpper->setText(QString::number(ui->horsliderValUpper->value()));


    ui->rButHue->setChecked(true);
    ui->rButFilter->setChecked(false);

    //connect(ui->rButHue,SIGNAL(pressed()),ui->rButFilter,SLOT(toggle()));
    //connect(ui->rButHue,SIGNAL(clicked(bool)),ui->rButHue,SLOT(setChecked(bool)));
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

        if(this->imageFileNames.size() > 0){

            this->imageFileNames.clear();

            QAbstractItemModel* model = new QStringListModel(this->imageFileNames);

            ui->listViewInputFileNames->setModel(model);

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


    // Get the root directory
    filterpath =   QFileDialog::getOpenFileName(this,"Open Filter File","/home/hakan/Development", tr("*.txt"));

    if(filterpath != NULL)
            ImageProcess::readFilter(filterpath, 29,false,false,true);




}

void ImageProcessDialog::on_butApplyAll_clicked()
{
    if(this->imageFileNames.size() <= 0) return;

    clock_t start,ends;

    if(ui->rButHue->isChecked())
    {

        qDebug()<<"Hue part is active";

        if(!DatabaseManager::isOpen())
        {

            qDebug()<<"Bubble Database Could not be opened!! returning...";

            return;

        }
        for(unsigned int i = 1; i <= imageFileNames.size(); i++)
        {


            QString fullPath = imageFileNames[i-1];

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
                return;
            }

            int satLower =  ui->horsliderSatLower->value();

            int satUpper =  ui->horsliderSatUpper->value();

            int valLower =  ui->horsliderValLower->value();

            int valUpper = ui->horsliderValUpper->value();

            Mat hueChannel= ImageProcess::generateChannelImage(img,0,satLower,satUpper,valLower,valUpper);

            QImage* image = new QImage(hueChannel.data,hueChannel.cols,hueChannel.rows,hueChannel.step,QImage::Format_Mono);

            ui->labelProcessedImage->setPixmap(QPixmap::fromImage(*image));

            start = clock();

            vector<bubblePoint> hueBubble = bubbleProcess::convertGrayImage2Bub(hueChannel,focalLengthPixels,180);

           // qDebug()<<resultt.size();

            vector<bubblePoint> reducedHueBubble = bubbleProcess::reduceBubble(hueBubble);

            ends = clock();

            qDebug()<<"Hue bubble generation time"<<((float)(ends-start)*1000/CLOCKS_PER_SEC)<<" ms";

            int noHarmonics = ui->lEditNoHarmonicsInvariant->text().toInt();

            if(ui->cBoxInvariants->isChecked())
            {
                qDebug()<<"Calculating invariants for hue";

                DFCoefficients dfcoeff = bubbleProcess::calculateDFCoefficients(reducedHueBubble,noHarmonics,noHarmonics);

                std::vector< std::vector<float> > invariants = bubbleProcess::calculateInvariants(reducedHueBubble, dfcoeff,noHarmonics, noHarmonics);

                DatabaseManager::insertInvariants(HUE_TYPE,frameNumber,invariants);

            }

            if(ui->cBoxBubbleStats->isChecked())
            {
                qDebug()<<"Calculating stats for hue";


             /************************** Perform filtering and obtain resulting mat images ***********************/
                 Mat satChannel= ImageProcess::generateChannelImage(img,1,satLower,satUpper,valLower,valUpper);
                 Mat valChannel= ImageProcess::generateChannelImage(img,2,satLower,satUpper,valLower,valUpper);
            /*****************************************************************************************************/

            /*************************** Convert images to bubbles ***********************************************/

                 vector<bubblePoint> satBubble = bubbleProcess::convertGrayImage2Bub(satChannel,focalLengthPixels,255);
                 vector<bubblePoint> valBubble = bubbleProcess::convertGrayImage2Bub(valChannel,focalLengthPixels,255);

            /*****************************************************************************************************/


                 /***************** Reduce the bubbles ********************************************************/
                 vector<bubblePoint> reducedSatBubble = bubbleProcess::reduceBubble(satBubble);
                 vector<bubblePoint> reducedValBubble = bubbleProcess::reduceBubble(valBubble);


                 // Calculate statistics
                 bubbleStatistics statsHue =  bubbleProcess::calculateBubbleStatistics(reducedHueBubble,180);
                 bubbleStatistics statsSat =  bubbleProcess::calculateBubbleStatistics(reducedSatBubble,255);
                 bubbleStatistics statsVal =  bubbleProcess::calculateBubbleStatistics(reducedValBubble,255);

                 DatabaseManager::insertBubbleStatistics(HUE_TYPE,frameNumber,statsHue);
                 DatabaseManager::insertBubbleStatistics(SAT_TYPE,frameNumber,statsSat);
                 DatabaseManager::insertBubbleStatistics(VAL_TYPE,frameNumber,statsVal);


            }

        }


        return;

    } // END HUE PART

    else if(ui->rButFilter->isChecked())
    {
        qDebug()<<"Filter part is active";

        if(!DatabaseManager::isOpen())
        {

            qDebug()<<"Database could not be opened!! returning...";

            return;
        }
        if(filters.size() == 0)
        {
            qDebug()<<"There are no filters selected!! returning...";

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

            ImageProcess::readFilter(filters.at(i),29,false,false,false);

            for(int j = 1; j <= this->imageFileNames.size(); j++)
            {

                QString tempPath = imageFileNames.at(j-1);

                qDebug()<<"Temp path: "<<tempPath;

                QString fullFilePath =tempPath;

                int frameNumber = ImageProcess::getFrameNumber(fullFilePath);

                if(frameNumber == -1){

                    qDebug()<<"Error!! Frame number could not be determined, returning...";
                    return;
                }


                start = clock();

                Mat img = ImageProcess::loadImage(tempPath,false);


                Mat resg;

                cv::cvtColor(img,resg,CV_BGR2GRAY);

                Mat sonuc = ImageProcess::applyFilter(resg);

                vector<bubblePoint> imgBubble = bubbleProcess::convertGrayImage2Bub(sonuc,focalLengthPixels,255);

                vector<bubblePoint> resred ;

                resred = bubbleProcess::reduceBubble(imgBubble);

                if(ui->cBoxInvariants->isChecked())
                {
                    int noHarmonics = ui->lEditNoHarmonicsInvariant->text().toInt();

                    DFCoefficients dfcoeff =  bubbleProcess::calculateDFCoefficients(resred,noHarmonics,noHarmonics);

                    std::vector< std::vector< float > > invariants =  bubbleProcess::calculateInvariants(resred,dfcoeff,noHarmonics,noHarmonics);



                    DatabaseManager::insertInvariants(filterNumber,frameNumber,invariants);
                }

                if(ui->cBoxBubbleStats->isChecked())
                {
                    qDebug()<<"Calculating stats for filter";

                    bubbleStatistics stats =  bubbleProcess::calculateBubbleStatistics(resred,255);

                    DatabaseManager::insertBubbleStatistics(filterNumber,frameNumber,stats);

                }

            } // END FOR

        } // END FOR

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

    if(!DatabaseManager::isOpen())
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

        Mat hueChannel= ImageProcess::generateChannelImage(img,0,satLower,satUpper,valLower,valUpper);

        QImage* image = new QImage(hueChannel.data,hueChannel.cols,hueChannel.rows,hueChannel.step,QImage::Format_Mono);

        ui->labelProcessedImage->setPixmap(QPixmap::fromImage(*image));

        start = clock();

        vector<bubblePoint> resultt = bubbleProcess::convertGrayImage2Bub(hueChannel,focalLengthPixels,180);

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

/*void ImageProcessDialog::on_butGenerateInvariants_clicked()
{
    /* QString path = ImageProcess::getDataSetPath();

    if(path == NULL) return;

    // QString fileName = ui->lEditCloudFileName->text();

    QDir dirPath(path);

    dirPath.setFilter(QDir::NoDotAndDotDot | QDir::Files);

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
    }

    if(!DatabaseManager::isOpen())
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


    }
}*/



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
        this->imageFileNames = dialog.selectedFiles();


    QAbstractItemModel* mod = new QStringListModel(this->imageFileNames);

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



void ImageProcessDialog::on_butRemoveInputFileNames_clicked()
{

    if(ui->listViewInputFileNames->model())
    {
        ui->listViewInputFileNames->model()->removeRow(ui->listViewInputFileNames->currentIndex().row());

        QStringListModel* sss = (QStringListModel*)ui->listViewInputFileNames->model();

        this->imageFileNames.clear();

        this->imageFileNames = sss->stringList();

        qDebug()<<this->imageFileNames;

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


/*void ImageProcessDialog::on_butGenerateHueHistograms_clicked()
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

}*/

/*void ImageProcessDialog::on_butGenerateHueHistBubble_clicked()
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

        cv::split(img,channels);






}*/

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
    Mat orgImg= ImageProcess::getImage();

    Mat loadedFilter = ImageProcess::getFilter();

    if(loadedFilter.empty()) return;

    if(orgImg.empty()) return;

    Mat grImg;

    cv::cvtColor(orgImg,grImg,CV_BGR2GRAY);

    Mat sonuc = ImageProcess::applyFilter(grImg);

    imwrite("filterResult.jpg",sonuc);

    vector<bubblePoint> initialBubble =  bubbleProcess::convertGrayImage2Bub(sonuc,focalLengthPixels,255);

    vector<bubblePoint> reducedBubble= bubbleProcess::reduceBubble(initialBubble);

    bubbleProcess::calculateBubbleStatistics(reducedBubble,255);

    QFile file("bubbleFiltResult.txt");

    file.open(QFile::WriteOnly);

    bubbleProcess::saveBubble(&file,reducedBubble);

    file.close();
}
