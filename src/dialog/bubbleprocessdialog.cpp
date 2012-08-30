#include "bubbleprocessdialog.h"
#include "ui_bubbleprocessdialog.h"
#include "bubbleprocess.h"
#include "bubbletransformer.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

vector<bubblePoint> aBubble;
int counter = 0;

BubbleProcessDialog::BubbleProcessDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BubbleProcessDialog)
{
    ui->setupUi(this);
    this->setAttribute(Qt::WA_DeleteOnClose);

    this->initializeView();

}

BubbleProcessDialog::~BubbleProcessDialog()
{
    delete ui;
}

void BubbleProcessDialog::on_But_bubbleFilesSetRootDir_clicked()
{
    // Get the root directory
    QString path =   QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                       "home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if(path != NULL){


        // append a slash
        path.append("/");

        // create a directory object with path
        QDir myDir(path);

        // Set the root directory for bubbleProcess
        bubbleProcess::setBubblesRootDirectory(path);

        ui->Edit_bubbleFilesRootDir->setText(path);

        // Create a filter for the given extensions
        QStringList filters(ui->Edit_bubbleFilesExtension->text());

        qDebug()<<filters;

        // List the bubbles in that directory
        // QStringList list = myDir.entryList(filters);

        // First check if we have a directory in the root file
        QStringList tempList = myDir.entryList(QDir::Dirs | QDir::NoDotDot |QDir::NoDot,QDir::Name);

        QStringList list =tempList;
        // If we have directory or directories (This part is specific to cx\apes0\bubble0.m format!!!)
        if(tempList.size() > 0){

            bool startsfromZero = false;

            for(unsigned int i = 0; i < tempList.size(); i++)
            {
                QString charr = "c";

                QString tmp = tempList.at(i);

                tmp.remove(0,1);

                int num = tmp.toInt();

                if(i==0 && i==num)startsfromZero = true;

                charr.append(tmp);

                if(!startsfromZero) list[num-1] = charr;

                else list[num] = charr;

            }
            // set the file list of the bubbles
            bubbleProcess::setBubblesFolderList(list);
        }
        else{

            // Create a filter for the given extensions
            QStringList filters(ui->Edit_bubbleFilesExtension->text());

            qDebug()<<filters;

            // List the bubbles in that directory
            QStringList list = myDir.entryList(filters);

            if(list.size()>0) bubbleProcess::setBubblesFileList(list);

        }


        qDebug()<<list;

    }



}
void BubbleProcessDialog::initializeView(){

    ui->Edit_bubbleFilesExtension->setText("bubble0.m");

    connect(ui->Widget_BPbubbleViewer, SIGNAL(frameChanged(const int &)),this, SLOT(handleGLBubbleViewerFrameChanged(const int &)));

}

void BubbleProcessDialog::on_But_fetchBubbleFiles_clicked()
{
    QString rootPath = bubbleProcess::getBubblesRootDirectory();

    // if rootPath is not set, there is nothing i can do!!
    if(rootPath == NULL)return;

    vector <vector <bubblePoint> > bubbles = bubbleProcess::getBubbles();

    // Clear previous bubbles, new ones are coming
    if(bubbles.size() > 0)bubbles.erase(bubbles.begin(),bubbles.end());

    // Get the folder List
    QStringList folderList = bubbleProcess::getBubblesFolderList();

    // create a new vector that will hold the read bubbles
    vector < vector < bubblePoint > > readBubbles;

    // This part is c0, c1, c2, ....
    if(folderList.size()>0){

        for(int i = 0; i < folderList.size(); i++){

            QString folderName = folderList.at(i);

            QString path = rootPath;

            // Create the path: "path/folder/"
            path.append(folderName).append("/");

            //qDebug()<<path;

            // create a directory object with path
            QDir myDir(path);

            // First check if we have a directory in the root file
            QStringList list = myDir.entryList(QDir::Dirs | QDir::NoDotDot |QDir::NoDot,QDir::NoSort);

            // This part is apes0
            if(list.size() > 0){

                for(int k = 0; k < list.size(); k++){

                    // Create a filter for the given extensions
                    QStringList filters(ui->Edit_bubbleFilesExtension->text());

                    // the path becomes "root/cx/apes0/"
                    path.append(list.at(k)).append("/");

                    //qDebug()<<filters;

                    //qDebug()<<path;

                    QDir dir(path);

                    // List the bubbles in that directory
                    QStringList list2 = dir.entryList(filters);


                    if(list2.size()>0){

                        for(int j = 0; j< list2.size(); j++){

                            QString totPath = path.append(list2.at(j));

                            qDebug()<<"Total Path is: "<<totPath;


                            QFile file(totPath);

                            file.open(QFile::ReadOnly);

                            vector<bubblePoint> aBubb =  bubbleProcess::readBubble(&file);

                            readBubbles.push_back(aBubb);

                            qDebug()<<"I have read a bubble";




                        }

                    }
                }


            }

        }


    }
    else{

        QString path = rootPath;

        // Create the path: "path/folder/"
        path.append("/");

        QStringList fileList = bubbleProcess::getBubblesFileList();

        if(fileList.size()>0){

            for(int i = 0; i < fileList.size(); i++){

                QString totPath = path.append(fileList.at(i));

                qDebug()<<"Total Path is: "<<totPath;


                QFile file(totPath);

                if(file.open(QFile::ReadOnly)){

                    vector<bubblePoint> aBubb =  bubbleProcess::readBubble(&file);

                    readBubbles.push_back(aBubb);
                }
            }

        }



    }

    bubbleProcess::setBubbles(readBubbles);


    bubbles = bubbleProcess::getBubbles();

    /// Fill the listview with loaded bubbles
    for(unsigned int i = 0; i < bubbles.size(); i ++)
    {
        ui->ComboBox_fetchedBubbles->addItem(QString::number(i));



    }

}

void BubbleProcessDialog::on_But_chooseFile_clicked()
{
    // Get the root directory
    QString path =   QFileDialog::getOpenFileName(this,"Open Bubble File",".",NULL);

    if(path != NULL)
    {

        QFile file(path);

        file.open(QFile::ReadOnly);

        aBubble =  bubbleProcess::readBubble(&file);

        // bubbleProcess.s

        qDebug()<<"I have read a bubble";

        ui->Widget_BPbubbleViewer->Points = aBubble;

        ui->Widget_BPbubbleViewer->setDrawType(DRAW_TYPE_DEFAULT);





    }


}
void BubbleProcessDialog::handleGLBubbleViewerFrameChanged(int frameNumber){

    //  QPixmap pixmap = QPixmap::grabWidget(ui->Widget_BPbubbleViewer);

    //   QString name = "frames/frame";

    //    name.append(QString::number(frameNumber));

    //name.append(".jpg");

    //  QString format = "PNG";

    //   pixmap.save(name,format.toAscii());

    //  ui->Widget_BPbubbleViewer->Points.clear();

    ui->Widget_BPbubbleViewer->Points.push_back(aBubble[counter]);

    counter++;

    if(counter > aBubble.size()){

        qDebug()<<"I have finished drawing";
        ui->Widget_BPbubbleViewer->setDrawType(DRAW_TYPE_NONE);

    }










}

void BubbleProcessDialog::on_ComboBox_fetchedBubbles_currentIndexChanged(int index)
{
    vector <vector < bubblePoint > > bubbles = bubbleProcess::getBubbles();

    if(bubbles.at(index).size() > 0){

        ui->Widget_BPbubbleViewer->Points = bubbles.at(index);

         ui->Widget_BPbubbleViewer->setDrawType(DRAW_TYPE_DEFAULT);
    }

}

void BubbleProcessDialog::on_But_readPoseData_clicked()
{

    // Get the root directory
    QString path =   QFileDialog::getOpenFileName(this,"Open Pose File",".",NULL);

    if(path != NULL)
    {

        QFile file(path);

        file.open(QFile::ReadOnly);

        vector<positionData> poses =  bubbleProcess::readPositionData(&file);

        qDebug()<<"I have read position data";

        bubbleProcess::setPositionData(poses);

       // bubbleProcess::se







    }

}

void BubbleProcessDialog::on_But_transformBubbles_clicked()
{
    QFile file("diffs.txt");

    vector<positionData> poses = bubbleProcess::getPositionData();

    if(poses.size() == 0) return;

    vector< vector < bubblePoint > > bubbles = bubbleProcess::getBubbles();

    if(bubbles.size() == 0) return;

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return;

    vector < vector <bubblePoint> > transformedBubbles;

    QTextStream out(&file);


    for(unsigned int i = 0; i < bubbles.size()-1; i++){

        vector<bubblePointXYZ> bubbleXYZ =  bubbleProcess::convertBubSph2BubXYZ(bubbles[i],15);

        positionData diff;

        diff.x = poses[i+1].x - poses[i].x;

        diff.y = poses[i+1].y - poses[i].y;

        diff.headingD = poses[i].headingD-poses[i+1].headingD;

        vector<bubblePoint> transformedBubble =  BubbleTransformer::transformXYZBubble(bubbleXYZ,diff,poses[i+1]);

        transformedBubbles.push_back(transformedBubble);

        vector< double > dif = bubbleProcess::calculateEuclideanDiff(bubbles[i+1],transformedBubble);

        qDebug()<<"diff is"<<dif[0]<<" "<<i+1<<" "<<i+2;

        // 1. sum over overlap 2. overlap 3. sum / count
        out<<dif[0]<<" "<<" "<<dif[1]<<" "<<dif[2]<<" "<< i+1 <<" "<<i+2<< "\n";
    }

    file.close();
    //ui->Widget_BPbubbleViewer->Points = transformedBubble;

   // ui->Widget_BPbubbleViewer->setDrawType(DRAW_TYPE_DEFAULT);
  //  bubbleProcess::

}

void BubbleProcessDialog::on_But_calcDiff_clicked()
{
    if(ui->Edit_transformSource->text() == NULL || ui->Edit_transformTarget->text() ==NULL) return;

    int source = ui->Edit_transformSource->text().toInt();

    int target = ui->Edit_transformTarget->text().toInt();


     vector<positionData> poses = bubbleProcess::getPositionData();

     vector< vector < bubblePoint > > bubbles = bubbleProcess::getBubbles();

     if(source < 0)return;
     else if(source > bubbles.size() ) return;

     if(target < 0)return;
     else if(target > bubbles.size() ) return;


   //  vector < vector <bubblePoint> > transformedBubbles;

        vector<bubblePointXYZ> bubbleXYZ =  bubbleProcess::convertBubSph2BubXYZ(bubbles[source],15);

        positionData diff;

        diff.x = poses[target].x - poses[source].x;

        diff.y = poses[target].y - poses[source].y;

        diff.headingD = poses[target].headingD-poses[source].headingD;

        if(diff.headingD > 359)diff.headingD -= 360;
        else if(diff.headingD < 0) diff.headingD += 360;

        vector<bubblePoint> transformedBubble =  BubbleTransformer::transformXYZBubble(bubbleXYZ,diff,poses[target]);

      //  transformedBubbles.push_back(transformedBubble);

        vector <double> res = bubbleProcess::calculateEuclideanDiff(bubbles[target],transformedBubble);

        qDebug()<<"diff is"<<res[0]<<" "<<source<<" "<<target;


    ui->Widget_BPbubbleViewer->Points = transformedBubble;

    ui->Widget_BPbubbleViewer->setDrawType(DRAW_TYPE_DEFAULT);
  //  bubbleProcess::

}
