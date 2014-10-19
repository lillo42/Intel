#include <QCoreApplication>
#include <QMetaType>

#include "ImagemController/imagecontroller.h"
#include "ImagemController/imagem.h"

#include <opencv/cv.h>

using namespace cv;
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qRegisterMetaType<Imagem>("Imagem");

    ImageController *i = new ImageController();
    i->start();

    return a.exec();
}
