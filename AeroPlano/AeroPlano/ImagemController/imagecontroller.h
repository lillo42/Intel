#ifndef IMAGECONTROLLER_H
#define IMAGECONTROLLER_H

#include <QObject>
#include <QRunnable>
#include <QThread>

#include "imagem.h"
#include "../TrataImage/trataimagecontroller.h"
#include "../TCP/tcpcontroller.h"
#include "../IO/iocontroller.h"

#include <opencv/cv.h>
using namespace cv;

class ImageController : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit ImageController(QObject *parent = 0);

    void start();
    void run();

signals:

public slots:

private slots:
    void onTerminouContagem(Imagem frame,int quantidade);
    void onRecebeImage(Mat frame);
    void onSaveImage(Mat frame, QString name);
private:
    TrataImageController *trata;
    TcpController *tcp;
    IOController *io;

    void addThreadPool();

    void criaTrataImageController();
    void criaIOController();
    void criaTcpController();

    void addImageTrata(Imagem image);
    void addImageTrata(Mat image);

    void processa();

    Imagem criaImagem(Mat frame);
    Imagem criaImagem(Mat frame,QString name);

};

#endif // IMAGECONTROLLER_H
