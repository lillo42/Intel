#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QMutex>
#include <QWaitCondition>
#include <QRunnable>
#include <QCoreApplication>

#include "save.h"
#include "read.h"
#include "../ImagemController/imagem.h"

#include <opencv/cv.h>
using namespace cv;

class IOController : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit IOController(QObject *parent = 0);

    void run();

    void stop();

    void addSave(Imagem frame);

    QString getImageName();

    Mat readImage(QString path);

signals:

public slots:

private:
    QWaitCondition sincronizedThread;

    Save saver;
    Read read;

    QString defaulName;
    QString extension;
    QString path;
    int count;

    QList<Imagem> listFrame;

    bool stopThread;

    void executeSave();
    void setVariable();
    void save(Imagem image);
};

#endif // IOCONTROLLER_H
