#include "waveletcontroller.h"

WaveletController::WaveletController(QObject *parent) :
    QObject(parent)
{
    w = new  Wavelet();
}

void WaveletController::addProcessa(Imagem frame)
{
    bool acorda = listaProcessa.isEmpty();
    listaProcessa.append(frame);
    if(acorda)
        sincronizaThread.wakeOne();
}

void WaveletController::run()
{
    processa();
}

void WaveletController::processa()
{
    QMutex lock;
   forever
   {
       executaWavelet();
       lock.lock();
       if(listaProcessa.isEmpty())
           sincronizaThread.wait(&lock);
       lock.unlock();
   }
}

void WaveletController::executaWavelet()
{
    Imagem image;
    while(!listaProcessa.empty())
    {
        image = listaProcessa.first();
        w->cvHaarWavelet(image.gpuFrame,image.gpuFrame,1);
        listaProcessa.removeFirst();
        emit onTerminouWavelet(image);
    }
}
