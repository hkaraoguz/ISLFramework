#include "bubbletransformer.h".h"
#include <QDebug>

using namespace cv;
using std::vector;
BubbleTransformer::BubbleTransformer()
{
    translationVector  =  cv::Mat::zeros(3,1,CV_32FC1);

    rotationMatrix = Mat::zeros(3,3,CV_32FC1);

    rotationMatrix.at<float>(0,0) = 9.9984628826577793e-01;

    rotationMatrix.at<float>(0,1) = 1.2635359098409581e-03;

    rotationMatrix.at<float>(0,2) = -1.7487233004436643e-02;

    rotationMatrix.at<float>(1,0) = -1.4779096108364480e-03;

    rotationMatrix.at<float>(1,1) = 9.9992385683542895e-01;

    rotationMatrix.at<float>(1,2) = -1.2251380107679535e-02;

    rotationMatrix.at<float>(2,0) = 1.7470421412464927e-02;

    rotationMatrix.at<float>(2,1) = 1.2275341476520762e-02;

    rotationMatrix.at<float>(2,2) = 9.9977202419716948e-01;

    qDebug()<<"Translation Vector is: "<<translationVector.at<float>(1,0);

    qDebug()<<"Rotation Matrix is: "<<rotationMatrix.at<float>(2,2);


}

vector <bubblePoint> BubbleTransformer::transformXYZBubble(vector<bubblePointXYZ> bubble, positionData diff, positionData pose){

    vector<bubblePoint> transformedBubble;



 //  diff.x = -1;
 //  diff.y = -1;
 //   diff.headingD = 0;
   // pose.x = 0;
   // pose.y = 0;

   // pose.headingD = 30;

    for(unsigned int i = 0; i < bubble.size(); i++)
    {
            bubblePointXYZ pt = bubble.at(i);

            bubblePoint resPt;

            double q1c = sqrt((pt.x - diff.x)*(pt.x - diff.x) + (pt.y - diff.y)*(pt.y - diff.y));

          //  qDebug()<<"ptx : "<<pt.x<<"pt.y "<<pt.y;

            double f1prime = atan2(pt.y - diff.y, pt.x - diff.x) ;

            f1prime -= diff.headingD*M_PI/180;

           // qDebug()<<"f1prime: "<<f1prime*180/3.14159;

            double f2prime = atan2(pt.z,q1c);

            double q1 = sqrt(pt.z*pt.z + q1c*q1c);

            f1prime = f1prime*180/3.14159;
            f2prime = f2prime*180/3.14159;



            if(f1prime < 0) f1prime += 360;
            else if(f1prime > 360) f1prime -=360;



            if(f2prime < 0) f2prime += 360;
            else if(f2prime > 360) f2prime -=360;

            q1 = q1/15;


            resPt.panAng = f1prime;
            resPt.tiltAng = f2prime;
            resPt.val = q1;

            transformedBubble.push_back(resPt);


    }


    return transformedBubble;



}

