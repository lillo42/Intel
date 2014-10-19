#include "waveletcontroller.h"

WaveletController::WaveletController(QObject *parent) :
    QObject(parent)
{
    w = new  Wavelet();
    this->setAutoDelete(true);
    finishedThread=false;
}

void WaveletController::addProcessa(Imagem frame)
{
    bool acorda = listaProcessa.isEmpty();
    listaProcessa.append(frame);
    if(acorda)
        sincronizedThread.wakeOne();
}

void WaveletController::stopThread()
{
    finishedThread=true;
    sincronizedThread.wakeOne();
}

void WaveletController::run()
{
    processImage();
}

void WaveletController::processImage()
{
   QMutex lock;
   while(!finishedThread)
   {
       executeWavelet();
       lock.lock();
       if(listaProcessa.isEmpty())
           sincronizedThread.wait(&lock);
       lock.unlock();
   }
}

void WaveletController::executeWavelet()
{
    Imagem image;
    while(!listaProcessa.empty())
    {
        image = listaProcessa.first();
        w->cvHaarWavelet(image.frame,image.frame,1);
        listaProcessa.removeFirst();
        emit onTerminouWavelet(image);
    }
}
