#include "hogcontroller.h"

HOGController::HOGController(QObject *parent) :
    QObject(parent)
{
    hog = new HOG();
    this->setAutoDelete(true);
    finishedThread = false;
}

void HOGController::run()
{
    processImage();
}

void HOGController::addProcessa(Imagem frame)
{
    bool acorda = listaProcessa.isEmpty();
    listaProcessa.append(frame);
    if(acorda)
        sincronizedThread.wakeOne();
}

void HOGController::stopThread()
{
    finishedThread = true;
    sincronizedThread.wakeOne();
}

void HOGController::processImage()
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

void HOGController::executeFilter()
{
    Imagem image;
    vector<Rect> found_filter;
    while(!listaProcessa.empty())
    {
        image = listaProcessa.first();
        hog->detecta(image.frame,found_filter);
        emit onFinishedHOG(image,found_filter);
        listaProcessa.removeFirst();
    }

}
