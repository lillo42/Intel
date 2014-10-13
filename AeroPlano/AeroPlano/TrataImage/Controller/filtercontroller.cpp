#include "filtercontroller.h"

FilterController::FilterController(QObject *parent) :
    QObject(parent)
{
    filter = new Filter();
    this->setAutoDelete(true);
    finishedThread = false;
}

void FilterController::addProcessa(Imagem frame)
{
    bool acorda = listaProcessa.isEmpty();
    listaProcessa.append(frame);
    if(acorda)
        sincronizedThread.wakeOne();
}

void FilterController::stopThread()
{
    finishedThread = true;
    sincronizedThread.wakeOne();
}

void FilterController::run()
{
    processImage();
}

void FilterController::processImage()
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

void FilterController::executeFilter()
{
    Imagem image;
    while(!listaProcessa.empty())
    {
        image = listaProcessa.first();
        filter->saltPepper(image.frame);
        listaProcessa.removeFirst();
        emit onTerminouSalEPimenta(image);
    }
}
