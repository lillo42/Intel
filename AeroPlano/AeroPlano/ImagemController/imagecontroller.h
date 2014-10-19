#ifndef IMAGECONTROLLER_H
#define IMAGECONTROLLER_H

#include <QObject>
#include <QRunnable>
#include <QThread>
#include <QMetaType>

#include <QTcpServer>
#include <QTcpSocket>

#include "imagem.h"
#include "../TrataImage/trataimagecontroller.h"
#include "../TCP/tcpcontroller.h"
#include "../IO/iocontroller.h"

#include <opencv/cv.h>
#include <opencv/highgui.h>
using namespace cv;

class ImageController : public QObject, public QRunnable
{
    Q_OBJECT

public:
    explicit ImageController(QObject *parent = 0);
    void run();
    void start();
signals:

public slots:

private slots:
    void onTerminouContagem(Imagem frame);

private:
    TrataImageController *trata;
    TcpController *tcp;
    IOController *io;

    void addThreadPool();

    void criaTrataImageController();
    void criaIOController();
    void criaTcpController();

    void addImageTrata(Imagem image);

    void processa();

    Imagem criaImagem(Mat frame);
};

#endif // IMAGECONTROLLER_H
