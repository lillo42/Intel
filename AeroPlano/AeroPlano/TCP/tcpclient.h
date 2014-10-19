#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QImage>
#include <QImageWriter>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);

    void star(QString ip,int port);
    void send(QImage image);
    void send(QByteArray data);

signals:

public slots:

private slots:
    void onReadReady();
    void onDisconnected();

private:
    QTcpSocket *socket;

    void createSocket();
    bool checkConnection();
};

#endif // TCPCLIENT_H
