#include "imageprocess.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

Mat filter;

Mat filterOrg;

ImageProcess::ImageProcess()
{

}

void ImageProcess::readFilter(QString fileName, int filterNum, int filterSize, bool transpose, bool save)
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


    if(transpose)
    cv::transpose(filterOrg,filterOrg);

    cv::convertScaleAbs(filterOrg,filter,128,128);

    namedWindow("filter");

    cv::Mat resizedFilter;

    cv::resize(filter,resizedFilter,resizedFilter.size(),5,5);

    imshow("filter",resizedFilter);

    waitKey();

    if(save)
    {

        imwrite("filt.jpg",resizedFilter);
        qDebug()<<"Filter image saved";
    }





}
Mat ImageProcess::loadImage(QString fileName)
{
    Mat result = imread(fileName.toStdString());

    return result;

}
Mat ImageProcess::applyFilter(Mat singleChannelImage)
{
    Mat result = Mat::zeros(singleChannelImage.rows,singleChannelImage.cols,CV_8UC1);

    cv::GaussianBlur(singleChannelImage,singleChannelImage,cv::Size(11,11),10,10);

    cv::filter2D(singleChannelImage,result,result.depth(),filterOrg);

  //  cv::threshold(result,result,250,255,CV_THRESH_BINARY);

  //  namedWindow("filterResult");

   // namedWindow("orgImage");

   // imshow("filterResult",result);

   // imshow("orgImage",singleChannelImage);

  //  waitKey();

    return result;


}
