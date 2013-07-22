#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H
#include <opencv2/opencv.hpp>
#include <QString>

using namespace cv;

class ImageProcess
{
public:

    ImageProcess();

   // Mat img;

    //Mat filter;

    static void readFilter(QString fileName, int filterNum, int filterSize, bool transpose, bool save, bool show);

    static Mat loadImage(QString fileName, bool show);

    static Mat applyFilter(Mat singleChannelImage);

    static void setDataSetPath(QString path);

    static QString getDataSetPath();

    static Mat generateHueImage(Mat image, int satLower, int satUpper, int valLower, int valUpper);

    static Mat generateHueImage(int satLower, int satUpper, int valLower, int valUpper);

    static void setImage(Mat image);

};

#endif // IMAGEPROCESS_H
