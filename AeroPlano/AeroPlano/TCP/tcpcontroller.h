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
#define PORT_IMAGE_NOT_PROCESS 20001
#define PORT_IMAGE_HOG 20002
#define PORT_IMAGE_PIXEL 20003

//TODO: See if is possible add in some list in IOController
class TcpController : public QObject
{
    Q_OBJECT
public:
    explicit TcpController(QObject *parent = 0);
    ~TcpController();

    void start();

    void sendImageHOG(Imagem image, int qtd);
    void sendImagePixel(Imagem image,int qtd);

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

    void sendImageNotProcess(Mat &frame);
    void sendImage(Mat &frame, int port);
    void sendImage(Mat &frame, int count,int port);

    void setVariable();
    TcpServer* searchTcpServer(int port);

    QByteArray mat2ByteArray(Mat &image);
};

#endif // TCPCONTROLLER_H
