#include "imagecontroller.h"

ImageController::ImageController(QObject *parent) :
    QObject(parent)
{
    criaTrataImageController();
    criaIOController();
    criaTcpController();
}

void ImageController::start()
{
    addThreadPool();
    tcp->start();
}

void ImageController::run()
{
    processa();
}

void ImageController::onTerminouContagem(Imagem frame)
{
    io->addSave(frame);
}

void ImageController::onRecebeImage(Mat frame)
{
    addImageTrata(frame);
}

void ImageController::addThreadPool()
{
    QThreadPool::globalInstance()->setMaxThreadCount(10);
    QThreadPool::globalInstance()->start(io);
    QThreadPool::globalInstance()->start(trata);
    QThreadPool::globalInstance()->start(this);
}

void ImageController::criaTrataImageController()
{
    trata = new TrataImageController(this);
    connect(trata,SIGNAL(onTerminouContagem(Imagem,int)),this,SLOT(onTerminouContagem(Imagem,int)));
}

void ImageController::criaIOController()
{
    io = new IOController(this);
}

void ImageController::criaTcpController()
{
    tcp = new TcpController();
    connect(tcp,SIGNAL(onRecebeFrame(Mat)),this,SLOT(onRecebeImage(Mat)));

}

void ImageController::addImageTrata(Imagem image)
{
    trata->addImage(image);
}

void ImageController::addImageTrata(Mat image)
{
    addImageTrata(criaImagem(image));
}

void ImageController::processa()
{

}

Imagem ImageController::criaImagem(Mat frame)
{
    Imagem i;
    i.nome = io->getImageName();
    i.frame = frame;
    return i;
}

Imagem ImageController::criaImagem(Mat frame, QString name)
{
    Imagem i;
    i.nome = name;
    i.frame = frame;
    return i;
}
