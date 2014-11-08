#include "iocontroller.h"

IOController::IOController(QObject *parent) :
    QObject(parent)
{
    setVariable();
}

IOController::~IOController()
{

}

void IOController::addSave(Imagem frame)
{
    save(frame);
}

void IOController::save(Imagem image)
{
    QString namePath =  path +  "/" + image.nome;
    saver.save(namePath,image.nome,image.frame,image.count);
}

QString IOController::getImageName()
{
    return defaulName + QString::number(++count) + extension;
}

QString IOController::getImageNameNotProcess()
{
    return defaulNameNotProcess + QString::number(++countNotProcess) + extension;
}

Mat IOController::readImage(QString path)
{
    return read.readImage(path.toStdString());
}

Mat IOController::readNextImage()
{

    QString imagePath = path + "/" + defaulName+QString::number(count) + extension;
    Mat retorno = imread(imagePath.toStdString().c_str(),CV_BGR2GRAY);
    if(!retorno.empty())
        count++;
    return retorno;
//    QImage image;
//    image.load(imagePath);
//    if(!image.isNull())
//        count++;
//    return cv::Mat(image.height(), image.width(),0, image.bits(), image.bytesPerLine());
}

void IOController::setVariable()
{
    defaulName = "Image_";
    extension = ".png";
    count = 0;
    countNotProcess =0;
    defaulNameNotProcess = "ImageP_";
    path = QCoreApplication::applicationDirPath() + "/Imagem";
}

