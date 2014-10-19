#ifndef TCPCONTROLLER_H
#define TCPCONTROLLER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QImage>
#include <QCoreApplication>
#include <QList>

#include "tcpserver.h"
#include "../ImagemController/imagem.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;
//TODO: See if is possible add in some list in IOController
class TcpController : public QObject
{
    Q_OBJECT
public:
    explicit TcpController(QObject *parent = 0);
    void start();

signals:

public slots:

private slots:
    void onReciveData(QByteArray data,int port);

private:
    QString textName;
    QString defaulName;
    QString extension;
    QString path;
    int count;
    QList<TcpServer*> listServes;

    QStringList readText();
    void startServers(QList<int> listPort);

    void imageRecive(QByteArray data);

    void setVariable();
};

#endif // TCPCONTROLLER_H
