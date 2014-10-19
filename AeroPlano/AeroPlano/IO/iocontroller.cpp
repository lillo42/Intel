#include "iocontroller.h"

IOController::IOController(QObject *parent) :
    QObject(parent)
{
    this->setAutoDelete(true);
    setVariable();
}

void IOController::run()
{
    QMutex lock;
    while(!stopThread)
    {
        executeSave();
        lock.lock();
        if(listFrame.isEmpty())
            sincronizedThread.wait(&lock);
        lock.unlock();
    }
}

void IOController::stop()
{
    stopThread = true;
    sincronizedThread.wakeOne();
}

void IOController::addSave(Imagem frame)
{
    bool wakeUp = listFrame.empty();
    listFrame.append(frame);
    if(wakeUp)
        sincronizedThread.wakeOne();
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

void IOController::executeSave()
{
    Imagem frame;
    QString name;
    while(!listFrame.isEmpty())
    {
        frame = listFrame.first();
        save(frame);
        listFrame.removeFirst();
    }
}

void IOController::setVariable()
{
    defaulName = "Image_";
    extension = ".jpg";
    count = 0;
    stopThread = false;
    path = QCoreApplication::applicationDirPath() + "/Imagem";
}

