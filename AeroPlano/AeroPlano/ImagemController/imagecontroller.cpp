#include "imagecontroller.h"

ImageController::ImageController(QObject *parent) :
    QObject(parent)
{
    criaTrataImageController();
    criaIOController();
    criaTcpController();
}

void ImageController::run()
{
    processa();
}

void ImageController::start()
{
    addThreadPool();
    tcp->start();
}

void ImageController::onTerminouContagem(Imagem frame)
{
    io->addSave(frame);
}


void ImageController::addThreadPool()
{
    QThreadPool::globalInstance()->setMaxThreadCount(10);
    QThreadPool::globalInstance()->start(trata);
    QThreadPool::globalInstance()->start(this);
}

void ImageController::criaTrataImageController()
{
    trata = new TrataImageController(this);
    connect(trata,SIGNAL(onTerminouContagem(Imagem)),this,SLOT(onTerminouContagem(Imagem)),Qt::DirectConnection);
}

void ImageController::criaIOController()
{
    io = new IOController(this);
}

void ImageController::criaTcpController()
{
   tcp = new TcpController();
}

void ImageController::addImageTrata(Imagem image)
{
    trata->addImage(image);
}


void ImageController::processa()
{
    while(true)
    {
        Mat frame;
        QThread::sleep(1);
        frame = io->readNextImage();
        if(frame.empty())
            continue;
        addImageTrata(criaImagem(frame));
    }
}

Imagem ImageController::criaImagem(Mat frame)
{
    Imagem i;
    i.nome = io->getImageName();
    i.frame = frame;
    return i;
}
