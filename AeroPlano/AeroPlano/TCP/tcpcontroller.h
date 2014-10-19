#ifndef TCPCONTROLLER_H
#define TCPCONTROLLER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QStringList>
#include <QFile>
#include <QImage>
#include <QCoreApplication>

#include "tcpserver.h"

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
    void onReciveFrame(Mat frame);
public slots:

private slots:
    void onReciveData(QByteArray data,int port);

private:
    QString textName;
    QString imageSaveName;
    QList<TcpServer*> listServes;

    QStringList readText();
    void startServers(QList<int> listPort);

    void convertQImageToMat(QImage image, Mat &frame);
    void imageRecive(QByteArray data);
};

#endif // TCPCONTROLLER_H
