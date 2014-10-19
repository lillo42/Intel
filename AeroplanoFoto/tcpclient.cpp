#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) :
    QObject(parent)
{
    socket = new QTcpSocket(this);
}

void TcpClient::star(QString ip, int port)
{
    socket->connectToHost(ip,port);
    socket->waitForConnected();
}

void TcpClient::send(QByteArray array)
{
    socket->write(array);
    socket->flush();
    socket->waitForBytesWritten();
}

void TcpClient::send(QImage image)
{

    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "jpg");
    send(ba);
//    QImageWriter writer;
//    writer.setDevice(socket);
//    writer.setFormat("jpg");

//   writer.write(image);
}
