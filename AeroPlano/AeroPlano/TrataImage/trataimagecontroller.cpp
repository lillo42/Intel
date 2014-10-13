#include "trataimagecontroller.h"

TrataImageController::TrataImageController(QObject *parent) :
    QObject(parent)
{
    criaLBP();
    criaSalEPimenta();
    criaWavelet();
    this->setAutoDelete(true);
}

TrataImageController::~TrataImageController()
{
    this->stopThread();
    filter->stopThread();
    lbp->stopThread();
    wavelet->stopThread();

    QThreadPool::globalInstance()->waitForDone(100);

    delete filter;
    delete lbp;
    delete wavelet;
}

void TrataImageController::addImage(Imagem frame)
{
    bool acorda = listaProcessa.isEmpty();
    //frame.gpuFrame = convertMatToGpuMat(frame.frame);
    listaProcessa.append(frame);
    if(acorda)
        sincronizaThread.wakeOne();
}

void TrataImageController::stopThread()
{
    finishedThread = true;
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
    //frame.frame = convertGpuMaToMat(frame.gpuFrame);
    lbp->addProcesa(frame);
}

void TrataImageController::criaLBP()
{
    lbp = new LBPController(this);
    connect(lbp,SIGNAL(onTerminouLBP(Imagem,int)),this,SLOT(OnTerminouLBP(Imagem,int)));
}

void TrataImageController::criaSalEPimenta()
{
    filter = new FilterController();
    connect(filter,SIGNAL(onTerminouSalEPimenta(Imagem)),this,SLOT(OnTerminouSalEPimenta(Imagem)));
}

void TrataImageController::criaWavelet()
{
    wavelet = new WaveletController();
    connect(wavelet,SIGNAL(onTerminouWavelet(Imagem)),this,SLOT(OnTerminouWavelet(Imagem)));
}

void TrataImageController::processa()
{
    QMutex lock;
    while(!finishedThread)
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
        filter->addProcessa(frame);
        listaProcessa.removeFirst();
    }
}

void TrataImageController::addProcessoThreadPool()
{
    QThreadPool::globalInstance()->start(wavelet);
    QThreadPool::globalInstance()->start(filter);
    QThreadPool::globalInstance()->start(lbp);
}
