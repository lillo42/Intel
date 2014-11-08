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
    ~TcpServer();

    void startServer(int port);

    int getPort();
    void sendaData(QByteArray &array);
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
    bool conectado;
};

#endif // TCPSERVER_H
