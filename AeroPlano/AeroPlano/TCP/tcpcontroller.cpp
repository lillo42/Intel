#include "tcpcontroller.h"

TcpController::TcpController(QObject *parent) :
    QObject(parent)
{
    setVariable();
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

void TcpController::imageRecive(QByteArray data)
{
    QImage image;
    image.loadFromData(data);
    if(image.isNull())
        return;
    else
        qDebug() << "Image not Null";
    QString localSave = path + "/" + defaulName + QString::number(count++) + extension;
    qDebug() << localSave;
    image.save(localSave,"JPG");
}

Imagem TcpController::createImagem(Mat &frame)
{
    Imagem i;
    i.frame = frame;
    return i;
}

void TcpController::setVariable()
{
    defaulName = "Image_";
    extension = ".jpg";
    count = 0;
    path = QCoreApplication::applicationDirPath() + "/Imagem";
    textName = QCoreApplication::applicationDirPath() + "/port.txt";
}
