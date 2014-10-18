#include <QCoreApplication>
#include <QThreadPool>


#include "camera.h"
#include "gps.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Camera *c = new Camera();
    GPS *g = new GPS();
    QThreadPool::globalInstance()->start(c);
    QThreadPool::globalInstance()->start(g);

    return a.exec();
}

