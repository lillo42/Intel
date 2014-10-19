#include "tcpcontroller.h"

TcpController::TcpController(QObject *parent) :
    QObject(parent)
{
    textName = QCoreApplication::applicationDirPath() + "/port.txt";
    imageSaveName = QCoreApplication::applicationDirPath() + "/imageMat.jpg";
}

void TcpController::start()
{
    QStringList listPortStrg = readText();
    QList<int> listPort;
    foreach (QString port, listPortStrg)
        listPort.append(port.toInt());
    startServers(listPort);
}

void TcpController::onReciveData(QByteArray data, int port)
{
    if(port == listServes[0]->getPort())//Port image
        imageRecive(data);
}

QStringList TcpController::readText()
{
    QFile txt(textName);
    txt.open(QIODevice::ReadOnly);

    QTextStream txtStrem(&txt);
    QStringList listPort;

    while(!txtStrem.atEnd())
        listPort.append(txtStrem.readLine());

    txt.close();
    return listPort;
}

void TcpController::startServers(QList<int> listPort)
{
    foreach (int port, listPort)
    {
        TcpServer *serve = new TcpServer(this);
        connect(serve,SIGNAL(reciveDate(QByteArray,int)),this,SLOT(onReciveData(QByteArray,int)));
        listServes.append(serve);
        serve->startServer(port);
    }
}

void TcpController::convertQImageToMat(QImage image, Mat &frame)
{
    image.save(imageSaveName);
    frame = imread(imageSaveName.toStdString());
}

void TcpController::imageRecive(QByteArray data)
{
    QImage image;
    image.loadFromData(data);
    if(image.isNull())
        qDebug() << "Image Null";
    return;
    Mat frame;
    convertQImageToMat(image,frame);

    emit onReciveFrame(frame);
}
