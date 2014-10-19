#include "iocontroller.h"

IOController::IOController(QObject *parent) :
    QObject(parent)
{
    setVariable();
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

Mat IOController::readImage(QString path)
{
    return read.readImage(path.toStdString());
}

Mat IOController::readNextImage()
{

    QString imagePath = path + "/" + defaulName+QString::number(count) + extension;
    QImage image;
    image.load(imagePath);
    if(!image.isNull())
        count++;
    return cv::Mat(image.height(), image.width(),0, image.bits(), image.bytesPerLine());
}

void IOController::setVariable()
{
    defaulName = "Image_";
    extension = ".jpg";
    count = 0;
    path = QCoreApplication::applicationDirPath() + "/Imagem";
}

