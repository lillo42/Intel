#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QCoreApplication>
#include <QDebug>
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QRunnable>
#include <QImage>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "tcpclient.h"

using namespace cv;
class Camera : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Camera(QObject *parent = 0);
    void run();
signals:

public slots:

private:
    QString txtFile;
    QString nameFile;

    void convertMatToQImage(Mat frame, QImage &retorno);
    bool canGetImage();
    void startConnection(TcpClient &tcp);
    void startCamera(VideoCapture &cam);

};

#endif // CAMERA_H
