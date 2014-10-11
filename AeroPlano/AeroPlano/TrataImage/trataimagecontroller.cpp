#include "trataimagecontroller.h"

TrataImageController::TrataImageController(QObject *parent) :
    QObject(parent)
{
    criaLBP();
    criaSalEPimenta();
    criaWavelet();
}

void TrataImageController::addImage(Imagem frame)
{
    bool acorda = listaProcessa.isEmpty();
    frame.gpuFrame = convertMatToGpuMat(frame.frame);
    listaProcessa.append(frame);
    if(acorda)
        sincronizaThread.wakeOne();
}

void TrataImageController::run()
{
    processa();
}

void TrataImageController::OnTerminouLBP(Imagem frame,int quantidade)
{
    emit onTerminouContagem(frame,quantidade);
}

void TrataImageController::OnTerminouSalEPimenta(Imagem salPimenta)
{
    wavelet->addProcessa(salPimenta);
}

void TrataImageController::OnTerminouWavelet(Imagem frame)
{
    frame.frame = convertGpuMaToMat(frame.gpuFrame);
    lbp->addProcesa(frame);
}

void TrataImageController::criaLBP()
{
    lbp = new LBPController(this);
    connect(lbp,SIGNAL(onTerminouLBP(Imagem,int)),this,SLOT(OnTerminouLBP(Imagem,int)));
}

void TrataImageController::criaSalEPimenta()
{
    sp = new SalEPimentaController();
    connect(sp,SIGNAL(onTerminouSalEPimenta(Imagem)),this,SLOT(OnTerminouSalEPimenta(Imagem)));
}

void TrataImageController::criaWavelet()
{
    wavelet = new WaveletController();
    connect(wavelet,SIGNAL(onTerminouWavelet(Imagem)),this,SLOT(OnTerminouWavelet(Imagem)));
}

void TrataImageController::processa()
{
    QMutex lock;
    forever
    {
        comecaProcessar();
        lock.lock();
        if(listaProcessa.isEmpty())
            sincronizaThread.wait(&lock);
        lock.unlock();
    }
}

void TrataImageController::comecaProcessar()
{
    Imagem frame;
    while(!listaProcessa.empty())
    {
        frame = listaProcessa.first();
        sp->addProcessa(frame);
        listaProcessa.removeFirst();
    }
}

void TrataImageController::addProcessoThreadPool()
{
    QThreadPool::globalInstance()->start(wavelet);
    QThreadPool::globalInstance()->start(sp);
    QThreadPool::globalInstance()->start(lbp);
}

gpu::GpuMat TrataImageController::convertMatToGpuMat(Mat frame)
{

}

Mat TrataImageController::convertGpuMaToMat(gpu::GpuMat frame)
{

}

