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
class TcpController : public QObject
{
    Q_OBJECT
public:
    explicit TcpController(QObject *parent = 0);
    void start();

signals:
    void onReciveFrame(Imagem frame);
    void onReciveQImage(QImage image);
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

    Imagem createImagem(Mat &frame);
    void setVariable();
};

#endif // TCPCONTROLLER_H
