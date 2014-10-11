#include "client.h"

Client::Client(QObject *parent) :
    QObject(parent)
{
    //this->socketDescriptor = ID;
    this->setAutoDelete(true);
    criaSocket();
}

void Client::run()
{
    while(!deleteThread)
    {

    }
}

void Client::readRead()
{

}

void Client::disconnect()
{
    deleteThread = true;
}

void Client::criaSocket()
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(readRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnect()));
}
