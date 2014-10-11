#include "salepimentacontroller.h"

SalEPimentaController::SalEPimentaController(QObject *parent) :
    QObject(parent)
{
    sp = new SalEPimenta();
}

void SalEPimentaController::addProcessa(Imagem frame)
{
    bool acorda = listaProcessa.isEmpty();
    listaProcessa.append(frame);
    if(acorda)
        sincronizaThread.wakeOne();
}

void SalEPimentaController::run()
{
    processa();
}

void SalEPimentaController::processa()
{
    QMutex lock;
    forever
    {
        executaSalPimentaLista();
        lock.lock();
        if(listaProcessa.isEmpty())
            sincronizaThread.wait(&lock);
        lock.unlock();
    }
}

void SalEPimentaController::executaSalPimentaLista()
{
    Imagem image;
    while(!listaProcessa.empty())
    {
        image = listaProcessa.first();
        sp->aplica(image.frame);
        listaProcessa.removeFirst();
        emit onTerminouSalEPimenta(image);
    }
}
