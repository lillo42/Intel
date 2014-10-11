#include "gravaimage.h"

GravaImage::GravaImage(QObject *parent) :
    QObject(parent)
{
    contador = -1;
}

QString GravaImage::GetPath()
{
    return this->path;
}

void GravaImage::SetPath(QString path)
{
    this->path = path;
}

int GravaImage::GetContador()
{
    return contador;
}

QString GravaImage::getNomePadrao()
{
    return NOME_IMAGEM;
}

void GravaImage::Grava(Mat frame)
{
    bool acorda = listaSalva.isEmpty();
    listaSalva.append(frame);

    if(acorda)
        sincronizaThread.wakeOne();
}

void GravaImage::Grava(QString nomeImage, Mat frame, int quantidade)
{

}

void GravaImage::run()
{
    QMutex lock;
    forever
    {
        GravaListaImagem();
        lock.lock();
        if(listaSalva.isEmpty())
            sincronizaThread.wait(&lock);
        lock.unlock();
    }
}

QString GravaImage::GetCaminhoImage()
{
    return path + NOME_IMAGEM + contador + EXTENSAO;
}

void GravaImage::GravaNoHD(Mat frame)
{
    contador++;
    imwrite(GetCaminhoImage().toStdString(),frame);
}

void GravaImage::GravaListaImagem()
{
    for(int i = 0; i < listaSalva.length();i++)
    {
        Mat frame = listaSalva.at(i);
        GravaNoHD(frame);
        emit OnGravou(frame);
        listaSalva.removeFirst();
    }
}
