#ifndef WAVELET_H
#define WAVELET_H


#include <cuda.h>


#include <iostream>
#include <vector>
#include <stdio.h>

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/gpu/gpumat.hpp>
using namespace cv;
using namespace std;

// Filter type
#define NONE 0  // no filter
#define HARD 1  // hard shrinkage
#define SOFT 2  // soft shrinkage
#define GARROT 3  // garrot filter

class Wavelet
{
public:
    Wavelet();

    // Wavelet transform
    void cvHaarWavelet(Mat &src,Mat &dst,int NIter);
    void cvHaarWavelet(Mat &src,Mat &dst,int NIter,vector<float> c,vector<float> dh,vector<float> dd,vector<float> dv);

    void cvHaarWavelets(Mat &src, float &c, float &dv, float &dh, float &dd, int x, int y);
private:

    // signum
    float sgn(float x);

    // Soft shrinkage
    float soft_shrink(float d,float T);

    // Hard shrinkage
    float hard_shrink(float d,float T);

    // Garrot shrinkage
    float garrot_shrink(float d,float T);
};

#endif // WAVELET_H
