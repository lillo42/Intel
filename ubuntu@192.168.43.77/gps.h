#ifndef GPS_H
#define GPS_H

#include <QObject>
#include <QRunnable>
#include <QFile>
#include <QTextStream>




#include "tcpclient.h"
class GPS : public QObject,public QRunnable
{
    Q_OBJECT
public:
    explicit GPS(QObject *parent = 0);
    void run();
signals:

public slots:

private:
    QString txtFile;

    void startConnection(TcpClient &tcp);
    void startSerial();
};

#endif // GPS_H
