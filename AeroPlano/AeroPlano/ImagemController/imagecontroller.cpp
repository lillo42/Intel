#include "imagecontroller.h"

ImageController::ImageController(QObject *parent) :
    QObject(parent)
{
    criaTrataImageController();
    //criaIOController();
}

void ImageController::start()
{
    addThreadPool();
}

void ImageController::run()
{
    processa();
}

void ImageController::onTerminouContagem(Imagem frame, int quantidade)
{
    io->gravar(frame.nome,frame.frame,quantidade);
}

void ImageController::onRecebeImage(Mat frame)
{
    addImageTrata(frame);
}

void ImageController::addThreadPool()
{
    QThreadPool::globalInstance()->start(this);
    QThreadPool::globalInstance()->start(trata);
    //QThreadPool::globalInstance()->start(io);
}

void ImageController::criaTrataImageController()
{
    trata = new TrataImageController(this);
    connect(trata,SIGNAL(onTerminouContagem(int)),this,SLOT(onTerminouContagem(int)));
}

void ImageController::criaIOController()
{
    //io = new IOController(this);
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
    i.nome = io->getNomePadrao() + QString::number(io->getContador());
    i.frame = frame;
    return i;
}
