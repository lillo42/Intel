#include "leimage.h"

LeImage::LeImage(QObject *parent) :
    QObject(parent)
{
    contador = 0;
}


Mat LeImage::leImagem(QString caminho)
{
    return imread(caminho.toUtf8().constData());
}

Mat LeImage::leProximaImagem()
{
    Mat retorno = leImagem(getPath());
    if(!retorno.empty())
        contador++;
    return retorno;
}

void LeImage::setPath(QString path)
{
    this->path = path;
}

QString LeImage::getPath()
{
    return path;
}

QString LeImage::getCaminho()
{
    return path + NOME_IMAGEM + contador + EXTENSAO;
}
