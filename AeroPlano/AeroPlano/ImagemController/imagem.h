#ifndef IMAGEM_H
#define IMAGEM_H

#include <QString>
#include <QMetaType>

#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
using namespace cv;
class Imagem
{
public:
    Imagem();
    ~Imagem();

    QString nome;
    Mat frame;
    int count;

};

Q_DECLARE_METATYPE(Imagem)

#endif // IMAGEM_H
