#include "lbpcontroller.h"

LBPController::LBPController(QObject *parent) :
    QObject(parent)
{
    lbp = new LBP();
}

void LBPController::addProcesa(Imagem frame)
{
    bool acorda = listaProcessa.isEmpty();
    listaProcessa.append(frame);
    if(acorda)
        sincronizaThread.wakeOne();
}

void LBPController::run()
{
    processa();
}

void LBPController::processa()
{
    QMutex lock;
    forever
    {
        executaLBP();
        lock.lock();
        if(listaProcessa.isEmpty())
            sincronizaThread.wait(&lock);
        lock.unlock();
    }
}

void LBPController::executaLBP()
{
    Imagem image;
    while(!listaProcessa.empty())
    {
        image = listaProcessa.first();
        int quantidade = lbp->Detecta(image.frame);
        listaProcessa.removeFirst();
        emit onTerminouLBP(image,quantidade);
    }
}
