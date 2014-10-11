#ifndef SALEPIMENTA_H
#define SALEPIMENTA_H

#include <opencv2/core/core.hpp>
#include <opencv/cv.h>
#include <opencv2/gpu/gpumat.hpp>
using namespace cv;

class SalEPimenta
{
public:
    SalEPimenta();

    void aplica(Mat frame);

    void aplica(gpu::GpuMat frame);
};

#endif // SALEPIMENTA_H
