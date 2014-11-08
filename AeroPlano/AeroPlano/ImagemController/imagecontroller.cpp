#include "imagecontroller.h"

ImageController::ImageController(QObject *parent) :
    QObject(parent)
{
    criaTrataImageController();
    criaIOController();
    criaTcpController();
}

ImageController::~ImageController()
{
    if(trata)
    {
        disconnect(trata,SIGNAL(onTerminouContagem(Imagem)));
        disconnect(trata,SIGNAL(onTerminouContagemHOG(Imagem,int)));
        disconnect(trata,SIGNAL(onTerminouContagemPixel(Imagem,int)));
        delete trata;
    }
    if(io)
    {
        delete io;
    }
    if(tcp)
    {
        delete tcp;
    }
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

void ImageController::onTerminouContagemHOG(Imagem frame, int cout)
{
    qDebug() << "Terminou de processar";
    tcp->sendImageHOG(frame,cout);
    io->addSave(frame);
}

void ImageController::onTerminouContagemPixel(Imagem frame, int cout)
{
    tcp->sendImagePixel(frame,cout);
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
    connect(trata,SIGNAL(onTerminouContagemHOG(Imagem,int)),this,SLOT(onTerminouContagemHOG(Imagem,int)));
    connect(trata,SIGNAL(onTerminouContagemPixel(Imagem,int)),this,SLOT(onTerminouContagemPixel(Imagem,int)));
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
        qDebug() << "Process";
        addImageTrata(criaImagem(frame));
    }
}

Imagem ImageController::criaImagem(Mat frame)
{
    Imagem i;
    i.nome = io->getImageNameNotProcess();
    i.frame = frame;
    i.frameNotProcess = frame.clone();
    return i;
}
