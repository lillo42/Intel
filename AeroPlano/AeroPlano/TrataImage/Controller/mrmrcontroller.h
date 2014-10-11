#ifndef MRMRCONTROLLER_H
#define MRMRCONTROLLER_H

#include <QObject>
#include <QRunnable>


#include <opencv/cv.h>
using namespace cv;

class MRMRController : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit MRMRController(QObject *parent = 0);

    void run();

signals:

public slots:

};

#endif // MRMRCONTROLLER_H
