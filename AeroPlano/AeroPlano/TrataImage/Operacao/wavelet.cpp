#include "wavelet.h"

Wavelet::Wavelet()
{
}

//--------------------------------
// signum
//--------------------------------
float Wavelet::sgn(float x)
{
    float res=0;
    if(x == 0)
        res = 0;
    else if(x > 0)
        res = 1;
    else if(x < 0)
        res = -1;
    return res;
}

//--------------------------------
// Soft shrinkage
//--------------------------------
float Wavelet::soft_shrink(float d, float T)
{
    float res;
    if(fabs(d) > T)
        res= sgn(d) * (fabs(d) - T);
    else
        res=0;
    return res;
}

//--------------------------------
// Hard shrinkage
//--------------------------------
float Wavelet::hard_shrink(float d, float T)
{
    float res;
    if(fabs(d) > T)
        res = d;
    else
        res = 0;
    return res;
}

//--------------------------------
// Garrot shrinkage
//--------------------------------
float Wavelet::garrot_shrink(float d, float T)
{
    float res;
    if(fabs(d) > T)
        res= d - ( (T * T) / d);
    else
        res=0;
    return res;
}

//--------------------------------
// Wavelet transform
//--------------------------------
void Wavelet::cvHaarWavelet(Mat &src, Mat &dst, int NIter)
{
    float c,dh,dv,dd;
    assert( src.type() == CV_32FC1 );
    assert( dst.type() == CV_32FC1 );
    int width = src.cols;
    int height = src.rows;
    for (int k = 0; k < NIter; k++)
    {
        for (int y = 0; y < (height >> (k + 1)); y++)
        {
            for (int x = 0; x< (width >> (k + 1));x++)
            {
                c = (src.at<float>(2 * y,2 * x) + src.at<float>(2 * y, 2 * x + 1) + src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
                dst.at<float>(y,x) = c;

                dh=(src.at<float>(2*y,2*x)+src.at<float>(2*y+1,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x+1))*0.5;
                dst.at<float>(y, x + (width >> (k+1))) = dh;

                dv=(src.at<float>(2*y,2*x)+src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)-src.at<float>(2*y+1,2*x+1))*0.5;
                dst.at<float>(y+( height >> (k+1)),x) = dv;

                dd=(src.at<float>(2*y,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
                dst.at<float>(y+(height >> (k+1)),x+(width >> (k+1))) = dd;
            }
        }
        dst.copyTo(src);
    }
}

void Wavelet::cvHaarWavelet(gpu::GpuMat &src, gpu::GpuMat &dst, int NIter)
{

}


