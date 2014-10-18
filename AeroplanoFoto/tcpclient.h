#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>
#include <QImage>
#include <QImageWriter>

class TcpClient : public QObject
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);
    void star(QString ip, int port);
    void send(QByteArray array);
    void send(QImage image);
signals:

public slots:

private slots:

private:
    QTcpSocket *socket;


};

#endif // TCPCLIENT_H
