#ifndef TCPCONTROLLER_H
#define TCPCONTROLLER_H

#include <QObject>

#include <opencv/cv.h>
using namespace cv;
class TcpController : public QObject
{
    Q_OBJECT
public:
    explicit TcpController(QObject *parent = 0);

    void start();
signals:
    void onRecebeFrame(Mat frame);
public slots:

};

#endif // TCPCONTROLLER_H
