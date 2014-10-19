#include "tcpclient.h"

TcpClient::TcpClient(QObject *parent) :
    QObject(parent)
{
}

void TcpClient::onReadReady()
{

}

void TcpClient::onDisconnected()
{
    disconnect(socket,SIGNAL(readyRead()));
    disconnect(socket,SIGNAL(disconnected()));
}

void TcpClient::createSocket()
{
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(readyRead()),this,SLOT(onReadReady()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(onDisconnected()));
}

bool TcpClient::checkConnection()
{
    if(socket->isOpen())
        return false;
    else if(socket->state() != QTcpSocket::ConnectingState || socket->state() !=  QTcpSocket::ConnectedState)
        return false;
    if(socket->state() == QTcpSocket::ConnectingState)
        socket->waitForConnected();
    return socket->isOpen();
}

void TcpClient::star(QString ip, int port)
{
    socket->connectToHost(ip,port);
}

void TcpClient::send(QByteArray data)
{
    if(!checkConnection())
    {
        qDebug() << "Connection problem";
        return;
    }

    socket->write(data);
    socket->flush();
    socket->waitForBytesWritten();
}

void TcpClient::send(QImage image)
{
    if(!checkConnection())
    {
        qDebug() << "Connection problem";
        return;
    }

    QImageWriter writer;
    writer.setDevice(socket);
    writer.setFormat("jpg");

    writer.write(image);
}
