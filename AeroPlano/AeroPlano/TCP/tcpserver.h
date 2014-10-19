#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

class TcpServer : public QObject
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    void startServer(int port);

    int getPort();
signals:
    void reciveDate(QByteArray data,int port);
public slots:

private slots:
    void newConnection();
    void readReady();
    void disconnected();
private:
    QTcpServer *server;
    QTcpSocket *socket;
    QByteArray array;
    int port;

    void createServer();
    void reciveImage();
    void reciveAnotherFing();
};

#endif // TCPSERVER_H
