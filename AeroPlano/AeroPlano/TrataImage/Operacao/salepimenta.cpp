#include "salepimenta.h"

SalEPimenta::SalEPimenta()
{
}

void SalEPimenta::aplica(Mat frame)
{
    Mat saltpepper_noise = Mat::zeros(frame.rows, frame.cols,CV_8U);
    randu(saltpepper_noise,0,255);

    Mat black = saltpepper_noise < 30;
    Mat white = saltpepper_noise > 225;

    frame.setTo(255,white);
    frame.setTo(0,black);
}

void SalEPimenta::aplica(gpu::GpuMat frame)
{

}
