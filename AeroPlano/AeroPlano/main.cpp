#include <QCoreApplication>
#include "ImagemController/imagecontroller.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ImageController *i = new ImageController();
    i->start();

    return a.exec();
}
