#include "tcpcontroller.h"

TcpController::TcpController(QObject *parent) :
    QObject(parent)
{
    setVariable();
}

TcpController::~TcpController()
{
    while(listServes.size() > 0)
    {
        TcpServer *s = listServes.first();
        listServes.removeFirst();
        disconnect(s,SIGNAL(reciveDate(QByteArray,int)));
        delete s;
    }
}


void TcpController::start()
{
    QStringList listPortStrg = readText();
    QList<int> listPort;
    foreach (QString port, listPortStrg)
        listPort.append(port.toInt());
    startServers(listPort);
}

void TcpController::sendImageHOG(Imagem image, int qtd)
{
   sendImageNotProcess(image.frameNotProcess);
   sendImage(image.frame,qtd,PORT_IMAGE_HOG);
   //TcpServer *s = searchTcpServer(port);
   //s->sendaData(image.nome.toLatin1());
   //s->sendaData(image.);

}

void TcpController::sendImagePixel(Imagem image, int qtd)
{
    sendImageNotProcess(image.frameNotProcess);
    sendImage(image.frame,qtd,PORT_IMAGE_PIXEL);
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
    image.save(localSave,"PNG");
}

void TcpController::sendImageNotProcess(Mat &frame)
{
    sendImage(frame,PORT_IMAGE_NOT_PROCESS);
}

void TcpController::sendImage(Mat &frame, int port)
{
    sendImage(frame,0,port);
}

void TcpController::sendImage(Mat &frame, int count, int port)
{
    TcpServer *s = searchTcpServer(port);
    QByteArray array = mat2ByteArray(frame);
    s->sendaData(array);
    array = QString::number(count).toLatin1();
    s->sendaData(array);
}


void TcpController::setVariable()
{
    defaulName = "Image_";
    extension = ".png";
    count = 0;
    path = QCoreApplication::applicationDirPath() + "/Imagem";
    textName = QCoreApplication::applicationDirPath() + "/port.txt";
}

TcpServer *TcpController::searchTcpServer(int port)
{
    TcpServer *s;
    for(int i = 0;i < listServes.size();i++)
    {
        s = listServes.at(i);
        if(s->getPort() == port)
            break;
    }
    return s;
}

QByteArray TcpController::mat2ByteArray(Mat &image)
{
      QByteArray byteArray;
//    QDataStream stream( &byteArray, QIODevice::WriteOnly );
//    stream << image.type();
//    stream << image.rows;
//    stream << image.cols;
//    const size_t data_size = image.cols * image.rows * image.elemSize();
//    QByteArray data = QByteArray::fromRawData( (const char*)image.ptr(), data_size );
//    stream << data;
    QImage i(image.data, image.cols, image.rows, image.step, QImage::Format_RGB32);
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    i.save(&buffer, "PNG");

    return byteArray;
}
