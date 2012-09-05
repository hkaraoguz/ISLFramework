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

    static void readFilter(QString fileName, int filterNum, int filterSize, bool transpose, bool save);

    static Mat loadImage(QString fileName);

    static Mat applyFilter(Mat singleChannelImage);
};

#endif // IMAGEPROCESS_H
