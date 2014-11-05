#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

#include <QObject>
#include <QString>
#include <QImage>
#include <QCoreApplication>

#include "save.h"
#include "read.h"
#include "../ImagemController/imagem.h"

#include <opencv/cv.h>
using namespace cv;

class IOController : public QObject
{
    Q_OBJECT
public:
    explicit IOController(QObject *parent = 0);
    ~IOController();

    void addSave(Imagem frame);

    QString getImageName();

    Mat readImage(QString path);
    Mat readNextImage();

signals:

public slots:

private:
    Save saver;
    Read read;

    QString defaulName;
    QString extension;
    QString path;
    int count;

    void setVariable();
    void save(Imagem image);
};

#endif // IOCONTROLLER_H
