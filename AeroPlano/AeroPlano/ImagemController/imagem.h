#ifndef IMAGEM_H
#define IMAGEM_H

#include <QString>

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/gpu/gpumat.hpp>
using namespace cv;
class Imagem
{
public:
    Imagem();

    QString nome;
    Mat frame;

};

#endif // IMAGEM_H
