#ifndef FILTER_H
#define FILTER_H

#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/gpu/gpumat.hpp>
using namespace cv;

class Filter
{
public:
    Filter();

    void saltPepper(Mat frame);
};

#endif // SALEPIMENTA_H
