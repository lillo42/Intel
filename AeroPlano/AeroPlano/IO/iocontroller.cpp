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

void IOController::addSave(Mat frame)
{
    bool wakeUp = listFrame.empty();
    listFrame.append(frame);
    if(wakeUp)
        sincronizedThread.wakeOne();
}

void IOController::save(QString name, int count)
{
    saver.save(path,name,count);
}

void IOController::save(Mat frame, QString name)
{
    string namePath =  path.toStdString() +  "/" + name.toStdString();
    saver.save(namePath,frame);
}

QString IOController::getImageName()
{
    return defaulName + QString::number(++count);
}

Mat IOController::readImage(QString path)
{

}

void IOController::executeSave()
{
    Mat frame;
    QString name;
    while(!listFrame.isEmpty())
    {
        frame = listFrame.first();
        name = getImageName();
        save(frame,name);
        listFrame.removeFirst();
        emit onSaveImage(frame,name);
    }
}

void IOController::setVariable()
{
    defaulName = "Image_";
    count = 0;
    stopThread = false;
    setDirector();
}

void IOController::setDirector()
{
    QString pathImage =  QDir::currentPath() + "/Image";
    QDir dirIimage(pathImage);
    if(!dirIimage.exists())
        dirIimage.mkdir(".");
    path = pathImage;
}
