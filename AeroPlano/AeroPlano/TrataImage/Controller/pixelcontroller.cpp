#include "pixelcontroller.h"

PixelController::PixelController(QObject *parent) :
    QObject(parent)
{
    pixel = new Pixel();
    this->setAutoDelete(true);
    finishedThread = false;
}

void PixelController::run()
{

}

void PixelController::addProcessa(Imagem frame)
{
    bool acorda = listaProcessa.isEmpty();
    listaProcessa.append(frame);
    if(acorda)
        sincronizedThread.wakeOne();
}

void PixelController::stopThread()
{
    finishedThread = true;
    sincronizedThread.wakeOne();
}

void PixelController::processImage()
{
    QMutex lock;
    while(!finishedThread)
    {
        executeFilter();
        lock.lock();
        if(listaProcessa.isEmpty())
            sincronizedThread.wait(&lock);
        lock.unlock();
    }
}

void PixelController::executeFilter()
{
    Imagem image;
    vector<Point> point;
    while(!listaProcessa.empty())
    {
        image = listaProcessa.first();
        pixel->detected(image.frame);//,&point);
        listaProcessa.removeFirst();

    }
}
