#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

//QT
#include <QObject>
#include <QRunnable>
#include <QMutex>
#include <QThreadPool>

//Minha Classe
#include "IO/gravaimage.h"
#include "IO/leimage.h"

//OpenCV
#include <opencv/cv.h>

class IOController : public QObject, public QRunnable
{
    Q_OBJECT

public:
    explicit IOController(QObject *parent = 0);

    int getContador();
    QString getNomePadrao();

    void SetPath(QString path);
    QString GetPath();

    void gravar(QString nome, Mat frame, int quantidade);

    Mat leImagem(QString caminho);
    Mat leProximaImage();
signals:
    void OnGravou(cv::Mat frame);
public slots:

private slots:

private:
    GravaImage* grava;
    LeImage le;
};

#endif // IOCONTROLLER_H
