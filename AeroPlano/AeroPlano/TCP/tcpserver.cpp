#include "tcpserver.h"

TcpServer::TcpServer(QObject *parent) :
    QObject(parent)
{
    createServer();
    conectado = false;
}

TcpServer::~TcpServer()
{
    if(socket)
    {
        socket->disconnectFromHost();
        disconnected();
        delete socket;
    }
    if(server)
    {
        server->close();
        disconnect(server,SIGNAL(newConnection()));
        delete server;

    }
}

void TcpServer::startServer(int port)
{
    this->port = port;
    QHostAddress ip("192.168.43.51");
    if(server->listen(ip,port))
        qDebug() << "Open port: " << port;
    else
        qDebug() << "Fail to open port: " << port;
}

int TcpServer::getPort()
{
    return port;
}

void TcpServer::sendaData(QByteArray &array)
{
    if(!conectado)
        return;
    if(socket->state() != QTcpSocket::ConnectedState)
        return;
    socket->write(array);
    socket->flush();
    socket->waitForBytesWritten();
}

void TcpServer::newConnection()
{
    qDebug() << "Nova conexao";
    conectado = true;
    socket = server->nextPendingConnection();
    connect(socket,SIGNAL(readyRead()),this,SLOT(readReady()));
    connect(socket,SIGNAL(disconnected()),this,SLOT(disconnected()));
}

void TcpServer::readReady()
{
    if(port == 20000)
        reciveImage();
    else
        reciveAnotherFing();
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

void TcpServer::reciveImage()
{
    array.append(socket->readAll());
    if(array.length() >= 35264)
    {
        emit reciveDate(array,port);
        array.clear();
    }
}

void TcpServer::reciveAnotherFing()
{

}
