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
        w->cvHaarWavelet(image.gpuFrame,image.gpuFrame,1);
        listaProcessa.removeFirst();
        emit onTerminouWavelet(image);
    }
}

void WaveletController::executeWavelet2()
{
    float c,dh,dv,dd;
    Imagem image;
    int NIter = 1;
    int width;
    int height;
    while(!listaProcessa.empty())
    {
        image = listaProcessa.first();
        width = image.frame.cols;
        height = image.frame.rows;
        for (int k = 0; k < NIter; k++)
        {
            for (int y = 0; y < (height >> (k + 1)); y++)
            {
                for (int x = 0; x< (width >> (k + 1));x++)
                {
                    w->cvHaarWavelets(image.frame,c,dv,dh,dd,x,y);
                    emit onCalculatesPoint(c);
                    emit onCalculatesPoint(dv);
                    emit onCalculatesPoint(dh);
                    emit onCalculatesPoint(dd);
                }
            }
        }
    }
}
