#include "imagecontroller.h"

ImageController::ImageController(QObject *parent) :
    QObject(parent)
{
    criaTrataImageController();
    criaIOController();
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
    //io->gravar(frame.nome,frame.frame,quantidade);
}

void ImageController::onRecebeImage(Mat frame)
{
    addImageTrata(frame);
}

void ImageController::onSaveImage(Mat frame, QString name)
{
    Imagem i = criaImagem(frame,name);
    trata->addImage(i);
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
    connect(io,SIGNAL(onSaveImage(Mat,QString)),this,SLOT(onSaveImage(Mat,QString)));
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
    //i.nome = io->getNomePadrao() + QString::number(io->getContador());
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
