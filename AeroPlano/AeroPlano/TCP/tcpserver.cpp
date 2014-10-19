#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) :
    QObject(parent)
{
}

void TcpServer::startServer(int port)
{
    this->port = port;
    if(server->listen(QHostAddress::Any,port))
        qDebug() << "Open port: " << port;
    else
        qDebug() << "Fail to open port: " << port;
}

int TcpServer::getPort()
{
    return port;
}

void TcpServer::newConnection()
{
    socket = server->nextPendingConnection();
    connect(socket,SIGNAL(readyRead()),this,SLOT(readReady()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
}

void TcpServer::readReady()
{
    QByteArray array;
    socket->waitForReadyRead();
    array.append(socket->readAll());
    emit reciveDate(array,port);
}

void TcpServer::disconnected()
{
    disconnect(socket,SIGNAL(readyRead()));
    disconnect(socket,SIGNAL(disconnected()));
}

void TcpServer::createServer()
{
    server = new QTcpServer();
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));
}
