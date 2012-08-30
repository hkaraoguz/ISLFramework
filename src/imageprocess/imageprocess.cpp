#include "imageprocess.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

Mat filter;

Mat filterOrg;

ImageProcess::ImageProcess()
{

}

void ImageProcess::readFilter(QString fileName, int filterNum, int filterSize)
{

    filterOrg = Mat(filterSize,filterSize,CV_32FC1);

    QString dirr = fileName;

    QString str;

    str.setNum(filterNum);

    dirr.append(str);

    dirr.append(".txt");

    qDebug()<<"Dir is :"<<dirr;


    QFile file(dirr);

    file.open(QFile::ReadOnly);

    QTextStream stream(&file);

    if(!file.isOpen()) return ;

    QString line = stream.readLine();

    double count = 0;

    double count2 = 0;

    while(line != NULL)
    {

        filterOrg.at<float>(count2,count) = line.toFloat();

        count++;


        if(count == filterSize){

            count2++;
            count = 0;

        }





        line = stream.readLine();

    }

    file.close();


    cv::transpose(filterOrg,filterOrg);

    cv::convertScaleAbs(filterOrg,filter,128,128);

    namedWindow("filter");

    imshow("filter",filter);

    waitKey();

   // imwrite("filt.jpeg",kkk);






}
Mat ImageProcess::loadImage(QString fileName)
{
    Mat result = imread(fileName.toStdString());

    return result;



}
Mat ImageProcess::applyFilter(Mat grayImage)
{
    Mat result = Mat::zeros(grayImage.rows,grayImage.cols,CV_8UC1);

    cv::GaussianBlur(grayImage,grayImage,cv::Size(11,11),10,10);

    cv::filter2D(grayImage,result,result.depth(),filterOrg);

  //  cv::threshold(result,result,250,255,CV_THRESH_BINARY);

  //  namedWindow("filterResult");

   // namedWindow("orgImage");

   // imshow("filterResult",result);

   // imshow("orgImage",grayImage);

  //  waitKey();

    return result;




}