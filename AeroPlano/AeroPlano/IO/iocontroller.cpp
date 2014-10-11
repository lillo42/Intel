#include "iocontroller.h"
IOController::IOController(QObject *parent) :
    QObject(parent)
{
    grava = new GravaImage();
    connect(this,SIGNAL(OnGravou(cv::Mat)),this,SIGNAL(OnGravou(cv::Mat)));
    QThreadPool::globalInstance()->start(grava);
}

int IOController::getContador()
{
    return grava->GetContador();
}

QString IOController::getNomePadrao()
{
    return grava->getNomePadrao();
}


void IOController::SetPath(QString path)
{
    grava->SetPath(path);
    le.setPath(path);
}

QString IOController::GetPath()
{
    return grava->GetPath();
}

void IOController::gravar(QString nome, Mat frame, int quantidade)
{

}

Mat IOController::leImagem(QString caminho)
{
    return le.leImagem(caminho);
}

Mat IOController::leProximaImage()
{
    return le.leProximaImagem();
}
