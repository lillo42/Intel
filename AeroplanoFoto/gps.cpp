#include "gps.h"

GPS::GPS(QObject *parent) :
    QObject(parent)
{
}

void GPS::run()
{

}

void GPS::startConnection(TcpClient &tcp)
{
    QFile file(txtFile);
    while(!file.open(QIODevice::ReadOnly))
        qDebug() << "Can't open file Image.txt";
    QTextStream text(&file);

    QStringList ipPort = text.readAll().split(";");
    tcp.star(ipPort[0],ipPort[1].toInt());

}

void GPS::startSerial()
{

}
