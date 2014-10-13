#ifndef IOCONTROLLER_H
#define IOCONTROLLER_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QMutex>
#include <QWaitCondition>
#include <QRunnable>


#include "save.h"
#include "read.h"

#include <opencv/cv.h>
using namespace cv;

class IOController : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit IOController(QObject *parent = 0);

    void run();

    void stop();

    void addSave(Mat frame);

    void save(QString name, int count);
    void save(Mat frame, QString name);

    QString getImageName();

    Mat readImage(QString path);

signals:
    void onSaveImage(Mat frame, QString name);
public slots:

private:
    QWaitCondition sincronizedThread;

    Save saver;
    Read read;

    QString defaulName;
    QString path;
    int count;

    QList<Mat> listFrame;

    bool stopThread;

    void executeSave();
    void setVariable();
    void setDirector();
};

#endif // IOCONTROLLER_H
