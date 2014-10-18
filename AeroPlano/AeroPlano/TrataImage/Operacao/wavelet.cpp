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

void Wavelet::cvHaarWavelet(Mat &src, Mat &dst, int NIter, vector<float> c, vector<float> dh, vector<float> dd, vector<float> dv)
{
    assert( src.type() == CV_32FC1 );
    assert( dst.type() == CV_32FC1 );
    int width = src.cols;
    int height = src.rows;
    float c2,dh2,dv2,dd2;
    for (int k = 0; k < NIter; k++)
    {
        for (int y = 0; y < (height >> (k + 1)); y++)
        {
            for (int x = 0; x< (width >> (k + 1));x++)
            {
                c2 = (src.at<float>(2 * y,2 * x) + src.at<float>(2 * y, 2 * x + 1) + src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
                //dst.at<float>(y,x) = c2;
                c.push_back(c2);

                dh2 = (src.at<float>(2*y,2*x)+src.at<float>(2*y+1,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x+1))*0.5;
                //dst.at<float>(y, x + (width >> (k+1))) = dh2;
                dh.push_back(dh2);

                dv2 = (src.at<float>(2*y,2*x)+src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)-src.at<float>(2*y+1,2*x+1))*0.5;
                //dst.at<float>(y+( height >> (k+1)),x) = dv2;
                dv.push_back(dv2);

                dd2 = (src.at<float>(2*y,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
                dst.at<float>(y+(height >> (k+1)),x+(width >> (k+1))) = dd2;
                dd.push_back(dd2);
            }
        }
        dst.copyTo(src);
    }
}

void Wavelet::cvHaarWavelets(Mat &src, float &c, float &dv, float &dh, float &dd, int x, int y)
{
    c = (src.at<float>(2 * y,2 * x) + src.at<float>(2 * y, 2 * x + 1) + src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;

    dh=(src.at<float>(2*y,2*x)+src.at<float>(2*y+1,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x+1))*0.5;

    dv=(src.at<float>(2*y,2*x)+src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)-src.at<float>(2*y+1,2*x+1))*0.5;

    dd=(src.at<float>(2*y,2*x)-src.at<float>(2*y,2*x+1)-src.at<float>(2*y+1,2*x)+src.at<float>(2*y+1,2*x+1))*0.5;
}




