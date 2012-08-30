#ifndef IMAGEPROCESS_H
#define IMAGEPROCESS_H
#include <opencv2/opencv.hpp>
#include <QString>

using namespace cv;

class ImageProcess
{
public:

    ImageProcess();

    Mat img;

    //Mat filter;

    static void readFilter(QString fileName, int filterNum, int filterSize);

    static Mat loadImage(QString fileName);

    static Mat applyFilter(Mat grayImage);
};

#endif // IMAGEPROCESS_H
