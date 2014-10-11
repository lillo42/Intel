#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QTcpServer>
#include <QByteArray>

class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);

signals:
      void onRecebeuDadas(QByteArray dados);
public slots:

private slots:

private:


};

#endif // TCPSERVER_H
