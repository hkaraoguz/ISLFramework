#ifndef BUBBLETRANSFORMER_H
#define BUBBLETRANSFORMER_H
#include "bubbleprocess.h"
#include <opencv2/opencv.hpp>



class BubbleTransformer
{
public:
    BubbleTransformer();

    static std::vector < bubblePoint > transformXYZBubble(vector<bubblePointXYZ> bubble, positionData diff,positionData pose );

      static std::vector < bubblePoint > transformXYZBubble(vector<bubblePointXYZ> bubble, positionData diff);


private:

    cv::Mat translationVector;

    cv::Mat rotationMatrix;


};

#endif // BUBBLETRANSFORMER_H
