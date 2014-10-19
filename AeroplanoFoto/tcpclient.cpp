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

void TcpClient::createSocket()
{

}

void TcpClient::send(QByteArray array)
{
    socket->write(array);
    socket->flush();
    socket->waitForBytesWritten();
}

void TcpClient::send(QImage image)
{
    if(socket->state() != QTcpSocket::ConnectedState)
    {
        qDebug() << "Nao connectado ainda";
        if(!socket->waitForConnected())
        {
            qDebug() << "Nao connectado";
            return;
        }
    }
    QImageWriter writer;
    writer.setDevice(socket);
    writer.setFormat("jpg");

   writer.write(image);
}
